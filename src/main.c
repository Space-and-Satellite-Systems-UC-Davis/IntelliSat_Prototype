#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>   // itimerval
#include <unistd.h>

#include "scheduler/scheduler.h"
#include "scheduler/task.h"
#include "scheduler/status.h"

/* Misc variables */
int reboot_count;   // TODO: get from FLASH
#define SYSTIMER_DUR 100000     // Config of system timer for background processses, in usec (100 ms)
#define BATTERY_THRESHOLD 20    // Min battery voltage value, below which mode -> CHARGING
volatile uint16_t flagBits = 0; // Seperate def. from declaration

jmp_buf toModeSelect;

// Global variables to track ISR calls
volatile int sys_handler_count = 0;
volatile int task_handler_count = 0;

//volatile struct Task currTask;

// Prototypes
void system_ISR_handler();
void task_ISR_handler();

/*
 * startup - when satellite boots up
 * (stall 30 min for first time boot, etc)
 * TODO: read reboot count from flash
 */
void startup() 
{
    if (reboot_count == 0) {
        // TODO: start 30 min if first boot
        SET_BIT(flagBits, START);
    }

    reboot_count++;
}

/*
 * loadBackups - retrieve critical info. from FLASH
 * (flagBits, reboot count, etc)
 */
void loadBackups() 
{
    printf("Loading Backups\n");
}

// Block the specified signal
void block_signal(int signal)
{
    // Define set of signals to block
    sigset_t sigset;

    // Initialize set to 0
    sigemptyset(&sigset);

    // Add given signal to set
    sigaddset(&sigset, signal);

    // Add sigset signals to process' blocked signals
    sigprocmask(SIG_BLOCK, &sigset, NULL); 
}

// Unblock a given signal
void unblock_signal(int signal)
{
    // Define set of signals to unblock
    sigset_t sigset;

    // Initialize set to 0
    sigemptyset(&sigset);

    // Add signal to set
    sigaddset(&sigset, signal);

    // Remove set signals from blocked signals
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

/*
 * system_ISR_handler - ISR for system/background tasks
 * (reflects kernel level code, for ECC, sensor polling, hardware checks)
 */
void system_ISR_handler(int signal)
{
   if(signal != SIGALRM) {
    return;
   }

    // Block task ISRs
    block_signal(SIGVTALRM);

    // Block other signals of the same type before updating count
    block_signal(SIGALRM);
    sys_handler_count++;
    unblock_signal(SIGALRM);

    //printf("**System ISR handler**\n");
    statusCheck();

    // Unblock task ISRs
    unblock_signal(SIGVTALRM);

    // siglongjmp(toModeSelect, flagBits);
}

/*
 * taskISR - used for preempting mode, based on varied time slice
 * (access to currTask, taskTable, etc possible through scheduler/scheduler.h)
 * TODO: Will go into interrupt handler file.
 */
void task_ISR_handler(int signal)
{
    if(signal != SIGVTALRM) {
        return;
    }

  //  printf("___________in Task ISR___________\n");

    // Block other signals of the same type before updating count
    block_signal(SIGVTALRM);
    task_handler_count++;
    unblock_signal(SIGVTALRM);

    //printf("------Task ISR handler------\n");
    int old_task_id = currTask.task_id;

    // Poll Sensors
    systemsCheck();
    modeSelect();

    if (old_task_id == currTask.task_id) {
        return;
    } else {
        taskTable[old_task_id].cleanPtr();
    }

    // Jmp using flagBits
    siglongjmp(toModeSelect, flagBits);
}

/* Run main() func., superloop after timer setup */
int main() 
{
    if (!IS_BIT_SET(flagBits, START)) {
        startup();
    } else {
        loadBackups();
    }

    /* System handler, timer setup */
    // Define signal handler and timer
    struct sigaction sys;
    struct itimerval sys_timer;

    // Install custom ISR as handler for SIGALRM
    sys.sa_handler = &system_ISR_handler;
    sigaction(SIGALRM, &sys, NULL);

    // Configure and start sys timer
    sys_timer.it_value.tv_sec = 0;
    sys_timer.it_value.tv_usec = SYSTIMER_DUR;
    sys_timer.it_interval.tv_sec = 0;
    sys_timer.it_interval.tv_usec = SYSTIMER_DUR;

    /* Task handler, timer setup */
    struct sigaction task;
    struct itimerval task_timer;

    // Install task_ISR_handler as signal handler for virtual (task) alarms
    task.sa_handler = &task_ISR_handler;
    sigaction(SIGVTALRM, &task, NULL);

    // // Configure task timer
    // TODO: need to do intervals?

    /* Set up rand function for testing */
	srand(2);
	printf("start\n");
	
    /* Run initial mode decision */
    systemsCheck(); // All other mode decisions done via task ISR
    
    /* Define jmp point */
    flagBits = sigsetjmp(toModeSelect, 1);

    /* Start sys timer */
    setitimer(ITIMER_REAL, &sys_timer, NULL);

    /* Run superloop */
    while (1) {
        printf("\n");

        modeSelect();
        
        // Configure task timer
        task_timer.it_value.tv_sec = 0;
        task_timer.it_value.tv_usec = currTask.timerDuration;
        task_timer.it_interval.tv_sec = 0;
        task_timer.it_interval.tv_usec = currTask.timerDuration;

    	printf("ID: %d\n", currTask.task_id);

        // Start virtual time for task
        setitimer(ITIMER_VIRTUAL, &task_timer, NULL);

        currTask.runPtr();  // usleep(rand) done here

        printf("Task %d is successful.\n", currTask.task_id);

	    CLR_BIT(flagBits, currTask.task_id);
        systemsCheck();

        printf("sys_handler_count: %d\n", sys_handler_count);
        printf("task_handler_count: %d\n", task_handler_count);
    }

    return 0;
}
