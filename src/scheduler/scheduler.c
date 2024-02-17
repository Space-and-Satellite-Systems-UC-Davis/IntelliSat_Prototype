/**
 * @file scheduler.c
 * @brief Scheduler logic
 *
 * Primary scheduling logic with mode switching, 
 * systems checking and mode selection.
 *
 * @authors Nithin Senthil, Parteek Singh, Jacob Tkeo
 * @date 9/8/23
 */

#include "scheduler.h"

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
    {ECC, 60000, 200, configEcc, ecc, cleanEcc},
    {HDD, 100000, 200, configHdd, hdd, cleanHdd},
	{MRW, 100000, 100, configMrw, mrw, cleanMrw}
};

volatile struct Task currTask;

uint32_t user_timeslice;
int scheduler_counter;
int task_counter;


 /**
 * @brief Signal block
 *
 * Blocks the specified interrupt signal by
 * by adding it to the curr. process blocked signals
 *
 * @param signal Signal to block
 * @see unblock_signal(), isSignalBlocked()
 * @warning Make sure a corresponding unblock is in place
 */
void block_signal(int signal) {
    sigset_t sigmyset;

    // Initialize set to 0
    sigemptyset(&sigmyset);

    // Add given signal to set
    sigaddset(&sigmyset, signal);

    // Add sigset signals to process' blocked signals
    sigprocmask(SIG_BLOCK, &sigmyset, NULL);
}


  /**
 * @brief Signal unblock
 *
 * Unblocks the specified interrupt signal by
 * by removing it from the curr. process blocked signals
 *
 * @param signal Signal to unblock
 * @see block_signal(), isSignalBlocked()
 */
void unblock_signal(int signal) {
    sigset_t sigmyset;

    // Initialize set to 0
    sigemptyset(&sigmyset);

    // Add signal to set
    sigaddset(&sigmyset, signal);

    // Remove set signals from blocked signals
    sigprocmask(SIG_UNBLOCK, &sigmyset, NULL);
}

 /**
 * @brief Checks if signal is blocked
 *
 * Checks if the specified signal is
 * on the curr. process blocked signals
 *
 * @param signal Signal to check
 * @see unblock_signal(), block_signal()
 */
int isSignalBlocked(int signal) {
    sigset_t sigmyset;
    sigprocmask(0, NULL, &sigmyset);
    return !sigismember(&sigmyset, signal);
}


/**
 * @brief Timeslice setter
 *
 * Sets the timeslice used in order to pre-empt the
 * runnning task.
 *
 * @param t timeslice in ms
 * @note Inputs must be a multiple of 10
 */
void set_user_timeslice(uint32_t t) {
    user_timeslice = t;
}


/**
 * @brief Update mode bits
 *
 * Checks every mode and updates mode bits
 * if the mode needs to be scheduled in.
 *
 * @see scheduler()
 * @note Implemented by relevant subteams
 * @todo Update for combining MRW and HDD
 *       Double check - mode peripheral requirements
 */
void systemsCheck() {
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


/**
 * @brief Selects highest priority mode
 *
 * Checks all the mode bits and selects the highest
 * priority mode that needs to run.
 *
 * @see scheduler()
 * @todo Update to new idle requirements
 */
void modeSelect() {

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

/**
 * @brief Handles common tasks during cleanup
 *
 * Uses a preference bitfield to determine which cleanup
 * actions need to be done including logs and clearing
 * the relevant bit from flagBits.
 *
 * @param field Preference field
 * @see scheduler()
 * @todo Update to match logging requirements
 */
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

/**
 * @brief Main scheduling logic
 *
 * Contains the timeslicing between system and mode time,
 * as well as manages the kill/preemption of modes.
 *
 * @param signal timer signal for blocking
 * @param toModeSelect jump buffer in case of preemption
 * @see sysTickHandler()
 * @todo Design check
 */
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
            // longjmp(*toModeSelect, flagBits); // SWITCH FOR HARDWARE INTEGRATION
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
}
