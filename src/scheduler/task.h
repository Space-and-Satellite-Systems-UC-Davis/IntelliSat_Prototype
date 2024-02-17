#ifndef _TASK_H_
#define _TASK_H_

#include "schedulerGlobals.h"

#include "status.h"

typedef void (*FunctionPointer)();  // For mode functions

/*
 * Task - a mode of execution on satellite
 * (note: uses preemption to interrupt running tasks)
 */
struct Task {
    uint8_t task_id;           // PRIMARY_KEY
    uint32_t timerDuration;     // in microseconds
    uint16_t taskInterrupts;    // times taskISR called, cancel mode/task after x reached
    FunctionPointer configPtr;  // configure timers, other mode info.
    FunctionPointer runPtr;     // the main func. for mode, via ADCS
    FunctionPointer cleanPtr;   // reset timers, clear temp buffers, etc
};

/* Scheduling methods */
int batteryTime(); // tautology (charging is idle mode)
int detumbleTime();
int commsTime();
int hddTime();
int mrwTime();
int eccTime();

/* Configure methods */
void configCharging();
void configDetumble();
void configComms();
void configHdd();
void configMrw();
void configEcc();

/* Run methods */
void charging();
void detumble();
void comms();
void hdd();
void mrw();
void ecc();

/* Clean methods */
void cleanCharging();
void cleanDetumble();
void cleanComms();
void cleanHdd();
void cleanMrw();
void cleanEcc();


#endif
