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
int batteryTime() {return !(rand() % 101);}    // tautology
int detumbleTime() {return !(rand() % 4);}
int commsTime() {return !(rand() % 4);} 
int hddTime() {return !(rand() % 4);}
int mrwTime() {return !(rand() % 4);}
int eccTime() {return !(rand() % 4);}

/* Configure methods */
void configCharging() {
    // Configure method for charging
    printf("Configure Charging is running\n");
}
void configDetumble() {
    // Configure method for detumble
}
void configComms() {
    // Configure method for comms
}
void configHdd() {
    // Configure method for hdd
}
void configMrw() {
    // Configure method for mrw
}
void configEcc() {
    // Configure method for ecc
}


/* Run methods */

void charging() {
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
void hdd() {
    // Run method for hdd
    printf("Run 'hdd'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void mrw() {
    // Run method for mrw
    printf("Run 'mrw'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void ecc() {
    // Run method for ecc
    printf("Run 'ecc'\n");
    usleep( ((rand() % 11) * 100000) + 10000 );
}


/* Clean methods */
void cleanCharging() {
    // Clean method for charging
    printf("cleanup ID: %d\n", CHARGING);
}
void cleanDetumble() {
    // Clean method for detumble
    printf("cleanup ID: %d\n", DETUMBLE);
}
void cleanComms() {
    // Clean method for comms
    printf("cleanup ID: %d\n", COMMS);
}
void cleanHdd() {
    // Clean method for hdd
    printf("cleanup ID: %d\n", HDD);
}
void cleanMrw() {
    // Clean method for mrw
    printf("cleanup ID: %d\n", MRW);
}
void cleanEcc() {
    // Clean method for ecc
    printf("cleanup ID: %d\n", ECC);
}
