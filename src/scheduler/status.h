#ifndef STATUS_H
#define STATUS_H
#pragma once

#include "schedulerGlobals.h"

/* Bitfield manipulation */
#define SET_BIT(BF, N) BF |= ((uint16_t) 0x01 << N)
#define CLR_BIT(BF, N) BF &= ~((uint16_t) 0x01 << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

/* Variable to store mode bits */
extern volatile uint16_t flagBits;

enum bits{
    CHARGING, DETUMBLE, COMMS, ECC, HDD, MRW,
    START, ANTENNA, COILS, HDD_STATUS, MRW_STATUS
};

/* 
	uint16_t flagBits:
Mode:
	Charging     0		Does the satellite need to charge?
	Detumble     1		Does the satellite need to detumble?
	Comms        2		Is there a downlink request?
	ECC          3		Is it time to perform ECC?
	HDD          4		Is an HDD test ready to run?
	MRW          5		Is an MRW test ready to run?

Status:
	Start        6		Have we completed the 30 minute timer?
	Antenna      7		Has antenna been deployed?
	Coils        8		Is the coils circuit communicating?
	HDD_status   9		Is the HDD ESC communicating?
	MRW_status   10		Is the MRW ESC communicating?
*/

void statusCheck();

#endif//STATUS_H
