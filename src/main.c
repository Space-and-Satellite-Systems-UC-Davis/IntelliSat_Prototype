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
#define SYSTIMER_DUR 100000     // Config. of system timer in usec (100 ms)
#define SYSTICK_DUR_U 1000        // Config. of systick timer in usec (1 ms)
#define BATTERY_THRESHOLD 20    // Min. battery voltage value, below which mode: CHARGING
volatile uint16_t flagBits = 0; // Separate def. from declaration
jmp_buf toModeSelect;

volatile int sys_handler_count = 0;	// Track ISR calls
volatile int task_handler_count = 0;
volatile int currentTaskInterrupts = 0;

// Temp working var
int systick_time;

/* Prototypes */
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

void sysTick_Handler(int signal) {
    // Backup guard from other alarms (Likely can be removed)
    if(signal != SIGALRM) {
        return;
    }

    systick_time++;
    scheduler(signal, &toModeSelect);
}

// /*
//  * system_ISR_handler - ISR for system/background tasks
//  * (reflects kernel level code, for ECC, sensor polling, hardware checks)
//  */
// void system_ISR_handler(int signal)
// {
//    if(signal != SIGALRM) {
//     return;
//    }

//     // Block task ISRs
//     block_signal(SIGVTALRM);

//     // Block other signals of the same type before updating count
//     block_signal(SIGALRM);
//     sys_handler_count++;
//     unblock_signal(SIGALRM);

//     statusCheck();

//     // Unblock task ISRs
//     unblock_signal(SIGVTALRM);

//     // siglongjmp(toModeSelect, flagBits);
// }

// /*
//  * taskISR - used for preempting mode, based on varied time slice
//  * (access to currTask, taskTable, etc possible through scheduler/scheduler.h)
//  * TODO: Will go into interrupt handler file.
//  */
// void task_ISR_handler(int signal)
// {
//     if(signal != SIGVTALRM) {
//         return;
//     }

//     // Block other signals of the same type before updating count
//     block_signal(SIGVTALRM);
//     currentTaskInterrupts++;
//     task_handler_count++;
//     unblock_signal(SIGVTALRM);

//     int old_task_id = currTask.task_id;

//     // Poll Sensors
//     systemsCheck();
//     modeSelect();

//     if (
//         old_task_id == currTask.task_id &&
//         currentTaskInterrupts < currTask.taskInterrupts
//     ) {
//         return;
//     } else {
//         currentTaskInterrupts = 0;
//         taskTable[old_task_id].cleanPtr();
//     }

//     // Jmp using flagBits
//     siglongjmp(toModeSelect, flagBits);
// }

/* Run superloop */
int main() 
{
    if (!IS_BIT_SET(flagBits, START)) {
        startup();
    } else {
        loadBackups();
    }

    /* System handler, timer setup */
    // Define signal handler and timer
    struct sigaction sysTick;
    struct itimerval sysTick_timer;

    // Install custom ISR as handler for SIGALRM
    sysTick.sa_handler = &sysTick_Handler;
    sigaction(SIGALRM, &sysTick, NULL);

    // Configure and start sys timer
    sysTick_timer.it_value.tv_sec = 0;
    sysTick_timer.it_value.tv_usec = SYSTICK_DUR_U;
    sysTick_timer.it_interval.tv_sec = 0;
    sysTick_timer.it_interval.tv_usec = SYSTICK_DUR_U;

    /* Set up rand function for testing */
	srand(2);
	printf("start\n");
	
    /* Run initial mode decision */
    systemsCheck(); // All other mode decisions done via task ISR
  
    /* Start sys timer */
    setitimer(ITIMER_REAL, &sysTick_timer, NULL);
    
    /* Define jmp point */
    sigsetjmp(toModeSelect, 1); // PROTOTYPE
    // setjmp(toModeSelect); SWITCH FOR FSW

    /* Run superloop */
    while (1) {
        printf("\n");

        modeSelect();

    	printf("ID: %d\n", currTask.task_id);

        currTask.runPtr();  // usleep(rand) done here

        printf("Task %d is successful.\n", currTask.task_id);

	    CLR_BIT(flagBits, currTask.task_id);

        // systemsCheck(); // REMOVE??

        printf("sysTick_handler_count: %d\n", systick_time);
    }

    return 0;
}
