/**
 * @file task.c
 * @brief All task related methods
 *
 * Contains methods pertaining run, clean,
 * and scheduling logic.
 *
 * @authors Nithin Senthil, Parteek Singh, Jacob Tkeo
 * @date 9/8/23
 */

#include "task.h"

/* Scheduling methods */
bool lowPwrTime() {
    // return !(rand() % 101);
    return 0;
}    // tautology
bool detumbleTime() {
    // return !(rand() % 4);
    return 1;
}
bool commsTime() {
    // return !(rand() % 4);
    return 1;
}

// adcs_mode ADCS_recommend_mode() {
//     return ADCS_DETUMBLE;
// }
int experimentTime() {
    // return !(rand() % 4);
    return ADCS_recommend_mode();
    // return 0;
}
bool eccTime() {
    // return !(rand() % 4);
    return 0;
}

/* Configure methods */
void configLowPwr() {
    // Configure method for charging
    printf("Configure Low Power is running\n");
}
void configDetumble() {
    // Configure method for detumble
}
void configComms() {
    // Configure method for comms
}
void configExperiment() {
    // Configure method for experiment
}
void configEcc() {
    // Configure method for ecc
}
void configIdle() {
}


/* Run methods */

void lowPwr() {
    // Run method for charging
    // usleep(rand());
    printf("Run 'charging'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void detumble() {
    // Run method for detumble
    // usleep(rand());
    printf("Run 'detumble'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void comms() {
    // Run method for comms
    printf("Run 'comms'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void experiment() {
    // Run method for experiment
    printf("Run 'experiment'\n");
    ADCS_MAIN(taskTable[4].func1);

    usleep( ((rand() % 11) * 100000) + 10000 );
}
void ecc() {
    // Run method for ecc
    printf("Run 'ecc'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void idle() {
    printf("Run Idle\n");
    while(1);
}


/* Clean methods */
void cleanLowPwr() {
    // Clean method for charging
    printf("cleanup ID: %d\n", LOWPWR);
}
void cleanDetumble() {
    // Clean method for detumble
    printf("cleanup ID: %d\n", DETUMBLE);
}
void cleanComms() {
    // Clean method for comms
    printf("cleanup ID: %d\n", COMMS);
}
void cleanExperiment() {
    // Clean method for experiment
    printf("cleanup ID: %d\n", EXPERIMENT);
}
void cleanEcc() {
    // Clean method for ecc
    printf("cleanup ID: %d\n", ECC);
}
void cleanIdle() {
}