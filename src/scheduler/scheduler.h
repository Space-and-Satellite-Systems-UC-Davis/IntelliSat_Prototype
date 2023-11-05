#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <setjmp.h>
#include "status.h"
#include "task.h"

extern volatile struct Task currTask;  // volatile: don't cache global var.
// TODO: error when defining this way, first building?

//jmp_buf toModeSelect;           // used to jmp back, save flagBits values

/* Task table */
// TODO: update to 5, remove ECC
// extern const struct Task taskTable[6];
extern const struct Task taskTable[];

void systemsCheck();
void modeSelect();

#endif // SCHEDULER_H
