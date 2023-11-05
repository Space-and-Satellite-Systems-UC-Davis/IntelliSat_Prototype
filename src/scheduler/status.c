#include "status.h"

/*
 * statusCheck - check peripherals, update status flags
 */
void statusCheck() {
    printf("Running status check\n");
    // Sets all status flags for testing
    for(int i = 7; i < 11; i++) {
        SET_BIT(flagBits, i);
    }
}
