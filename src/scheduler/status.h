#ifndef STATUS_H
#define STATUS_H
#pragma once        // ???

#include <stdint.h> // uint16_t
#include <stdio.h>

/* Bitfield manipulation */
#define SET_BIT(BF, N) BF |= ((uint16_t) 0x01 << N)
#define CLR_BIT(BF, N) BF &= ~((uint16_t) 0x01 << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

/* Var. to store mode bits */
// volatile uint16_t flagBits;
extern volatile uint16_t flagBits;

enum bits{
    CHARGING, DETUMBLE, COMMS, HDD, MRW, ECC,
    START, ANTENNA, COILS, HDD_STATUS, MRW_STATUS
};

/* 
uint16_t flagBits:

Charging     0		Does the satellite need to charge?
Detumble     1		Does the satellite need to detumble?
Comms        2		Is there a downlink request?
HDD          3		Is an HDD test ready to run?
MRW          4		Is an MRW test ready to run?
ECC          5		Is it time to perform ECC?
--------- Mode ^^^ | vvv Status ----------------------------
Start        6		Has initial 30 min timer run or not?
Antenna      7		Has antenna been deployed?
Coils        8		Is the coils circuit communicating?
HDD_status   9		Is the HDD ESC communicating?
MRW_status   10		Is the MRW ESC communicating?
*/

void statusCheck();

#endif//STATUS_H
