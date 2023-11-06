#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <setjmp.h>
#include "status.h"
#include "task.h"

extern volatile struct Task currTask;  // volatile: don't cache global var.

/* Task table */
extern const struct Task taskTable[];

void systemsCheck();
void modeSelect();

#endif // SCHEDULER_H
