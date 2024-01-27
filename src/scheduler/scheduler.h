#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include "status.h"
#include "task.h"

extern volatile struct Task currTask;  // volatile: don't cache global var.

/* Task table */
extern const struct Task taskTable[];

void block_signal(int signal);
void unblock_signal(int signal);
bool isSignalBlocked(int signal);
void set_user_timeslice(uint32_t t);

void systemsCheck();
void modeSelect();
void cleanup_handler(int8_t field);
void scheduler(int signal, jmp_buf* toModeSelect);

#endif // SCHEDULER_H
