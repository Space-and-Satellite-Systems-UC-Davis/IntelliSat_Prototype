#include "task.h"

/* Scheduling methods */
bool batteryTime() {return !(rand() % 101);}    // tautology
bool detumbleTime() {return !(rand() % 4);}
bool commsTime() {return !(rand() % 4);} 
bool hddTime() {return !(rand() % 4);}
bool mrwTime() {return !(rand() % 4);}
bool eccTime() {return !(rand() % 4);}

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

    usleep( ((rand() % 11) * 100000) + 10000 );
}
void detumble() {
    // Run method for detumble
    // usleep(rand());
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void comms() {
    // Run method for comms
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void hdd() {
    // Run method for hdd
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void mrw() {
    // Run method for mrw
    usleep( ((rand() % 11) * 100000) + 10000 );
}
void ecc() {
    // Run method for ecc
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
