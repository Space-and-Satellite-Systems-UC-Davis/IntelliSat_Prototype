#include "scheduler.h"
#include "status.h"

#define SYSTICK_DUR_M 100     // Config. of system timer in usec (100 ms)

/*
 * taskTable - info. about all modes on satellite
 * (mode, timerDur, numInterrupts, & corresponding functions)
 */

// TODO: update ECC mode based on discussions with ELEC
// RS - system, Hamming codes on Idle
const struct Task taskTable[] = {
	{CHARGING, 50000, 10000, configCharging, charging, cleanCharging},
	{DETUMBLE, 50000,10000, configDetumble, detumble, cleanDetumble},
	{COMMS, 100000, 500, configComms, comms, cleanComms},
    {HDD, 100000, 200, configHdd, hdd, cleanHdd},
	{MRW, 100000, 100, configMrw, mrw, cleanMrw},
    {ECC, 60000, 200, configEcc, ecc, cleanEcc}
};

volatile struct Task currTask;

uint32_t user_timeslice;
int scheduler_counter;
int task_counter;


/*
 * block_signal - add signals to curr. process blocked signals
 * @param signal: signal to block
 */
void block_signal(int signal)
{

    sigset_t sigmyset;

    // Initialize set to 0
    sigemptyset(&sigmyset);

    // Add given signal to set
    sigaddset(&sigmyset, signal);

    // Add sigset signals to process' blocked signals
    sigprocmask(SIG_BLOCK, &sigmyset, NULL);
}

// Unblock a given signal
/*
 * unblock_signal - remove signals from process blocked set
 * @param signal: signal to unblock
 */
void unblock_signal(int signal)
{
    sigset_t sigmyset;

    // Initialize set to 0
    sigemptyset(&sigmyset);

    // Add signal to set
    sigaddset(&sigmyset, signal);

    // Remove set signals from blocked signals
    sigprocmask(SIG_UNBLOCK, &sigmyset, NULL);
}

bool isSignalBlocked(int signal)
{
    sigset_t sigmyset;
    sigprocmask(0, NULL, &sigmyset);
    return !sigismember(&sigmyset, signal);

}

/*
 * Sets the Timeslice used in order to pre-empt the running task
 * NOTE: Inputs must be a multiple of 10
 * 
 * @param    t   timeslice in ms
 * @returns none
 */
void set_user_timeslice(uint32_t t) {
    user_timeslice = t;
}

/*
 * systemsCheck - update mode bits
 * TODO: Add in additional conditions for selecting modes
 * 	(ex. detumble needs coils to be working)
 */
void systemsCheck()
{
    statusCheck(); // CHARGING flag updated in statusCheck()

    if (detumbleTime()) {
        if (IS_BIT_SET(flagBits, COILS)) 
                SET_BIT(flagBits, DETUMBLE);
        else
            printf("Coils circuit is not communicating");
    } else {
	    CLR_BIT(flagBits, DETUMBLE);
    }

    if (commsTime()) {
        if (IS_BIT_SET(flagBits, ANTENNA))
            SET_BIT(flagBits, COMMS);
        else
            printf("Antenna is not deployed\n");
    } else {
        CLR_BIT(flagBits, COMMS);
    }

    if (hddTime()) {
        if (IS_BIT_SET(flagBits, HDD_STATUS))
            SET_BIT(flagBits, HDD);
        else
            printf("The HDD ESC is not communicating\n");
    } else {
	    CLR_BIT(flagBits, HDD);
    }

    if (mrwTime()) {
        if (IS_BIT_SET(flagBits, MRW_STATUS))
            SET_BIT(flagBits, MRW);
        else
            printf("The MRW ESC is not communicating\n");
    } else {
	    CLR_BIT(flagBits, MRW);
    }

    if (eccTime())
        SET_BIT(flagBits, ECC);
    else
	    CLR_BIT(flagBits, ECC);
}

/*
 * modeSelect - choose mode based on mode bits only
 * (bits 0-5 of flagBits bitfield correspond to mode bits)
 */
void modeSelect()
{
    // Initialize currTask
    currTask = taskTable[0]; 

    // Counters
    int new_task_id = 0;
    int i;

    // Determine which mode to run next
    for (i = 0; i < 6; i++) {
        if (IS_BIT_SET(flagBits, i)) {
            new_task_id = i;
            break;
        }
    }

    // Idle mode = CHARGING
    if (i == 6) {
        printf("Idle\n");
        new_task_id = 0;
    }
    
    // Select task to run from taskTable 
    currTask = taskTable[new_task_id];

}

void cleanup_handler(int8_t field) {
    // b0 - flagBit reset
    // b1 - success log

    if (IS_BIT_SET(field, 0)) {
        CLR_BIT(flagBits, currTask.task_id);
    }
    if (IS_BIT_SET(field, 1)) {
        printf("Task ID: %d has been logged\n", currTask.task_id);
    }

    currTask.cleanPtr();
}

void scheduler(int signal, jmp_buf* toModeSelect) {
    // isSignalBlocked guard not required if blocking signals during system time
    // if (isSignalBlocked(signal)) {
    //     return;
    // }

    scheduler_counter++;
    task_counter++;

    if (!(scheduler_counter % SYSTICK_DUR_M)) {
        block_signal(signal);
        
        // Store current task_id
        int old_task_id = currTask.task_id;
        
        // Update flags and reselect task
        systemsCheck();
        modeSelect();

        // If same task is selected, resume execution
        if (old_task_id == currTask.task_id) {
            return;
        } else {
            //Preempt
            printf("Task %d preempted by %d\n", old_task_id, currTask.task_id);
            cleanup_handler(0b00);

            // Jmp using flagBits
            siglongjmp(*toModeSelect, flagBits); // PROTOTYPE
            // longjmp(*toModeSelect, flagBits); // SWITCH FOR FSW
        }
        scheduler_counter = 0;

        unblock_signal(signal);
    }

    if (task_counter % user_timeslice) {
        block_signal(signal);

        task_counter = 0;
        //kill
        cleanup_handler(0b10);

        unblock_signal(signal);
    }

    
    

    // currentTaskInterrupts++;
    // task_handler_count++;
    // unblock_signal(SIGVTALRM);

    // int old_task_id = currTask.task_id;

    // // Poll Sensors
    // systemsCheck();
    // modeSelect();

    // if (
    //     old_task_id == currTask.task_id &&
    //     currentTaskInterrupts < currTask.taskInterrupts
    // ) {
    //     return;
    // } else {
    //     currentTaskInterrupts = 0;
    //     taskTable[old_task_id].cleanPtr();
    // }

    // // Jmp using flagBits
    // siglongjmp(toModeSelect, flagBits);
}
