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


#if KERNEL_VERSION == 1

/* Scheduling methods */
bool lowPwrTime() {
    // return !(rand() % 101);
    return false;
}    // tautology
bool detumbleTime() {
    // return !(rand() % 4);
    return false;
}
bool commsTime() {
    // return !(rand() % 4);
    return false;
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
    return false;
}

#else

/* Scheduling methods */
bool lowPwrTime() {
    // return !(rand() % 101);
    return false;
}
bool detumbleTime() {
    // return !(rand() % 4);
    return true;
}
bool commsTime() {
    // return !(rand() % 4);
    return true;
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
    return false;
}
#endif


/* Configure methods */
void configLowPwr() {
    // Configure method for charging
    // printMsg("Configure Low Power is running\n");
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
    // printMsg("Run 'charging'\n");

    led_dx(0, 1);
    usleep( ((rand() % 11) * 100000) + 10000 );
}
    // Run method for detumble
void detumble() {
    // usleep(rand());
    // printMsg("Run 'detumble'\n");

    led_dx(1, 1);
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void comms() {
    // Run method for comms
    // printMsg("Run 'comms'\n");

    led_dx(2, 1);
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void experiment() {
    // Run method for experiment
    // printMsg("Run 'experiment'\n");

    led_dx(3, 1);
    ADCS_MAIN(taskTable[4].func1);

    usleep( ((rand() % 11) * 100000) + 10000 );
}
void ecc() {
    // Run method for ecc
    // printMsg("Run 'ecc'\n");

    led_a(1);
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void idle() {
    // printMsg("Run Idle\n");

    led_b(1);
    while(1);
}


/* Clean methods */
void cleanLowPwr() {
    // Clean method for charging
    // printMsg("cleanup ID: %d\n", LOWPWR);

    led_dx(0, 0);
}
void cleanDetumble() {
    // Clean method for detumble
    // printMsg("cleanup ID: %d\n", DETUMBLE);

    led_dx(1, 0);
}
void cleanComms() {
    // Clean method for comms
    // printMsg("cleanup ID: %d\n", COMMS);

    led_dx(2, 0);
}
void cleanExperiment() {
    // Clean method for experiment
    // printMsg("cleanup ID: %d\n", EXPERIMENT);

    led_dx(3, 0);
}
void cleanEcc() {
    // Clean method for ecc
    // printMsg("cleanup ID: %d\n", ECC);

    led_a(0);
}
void cleanIdle() {
    led_b(0);
}