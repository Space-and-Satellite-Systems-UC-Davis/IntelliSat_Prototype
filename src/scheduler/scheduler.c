#include "scheduler.h"
#include "status.h"

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

//volatile struct Task currTask = taskTable[0];

volatile struct Task currTask;

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
