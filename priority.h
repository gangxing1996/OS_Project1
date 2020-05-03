//priority.h
#ifndef PRIORITY_H
#define PRIORITY_H

#include <sys/types.h> // pid_t

void choose_core(pid_t pid, int idx_core);
void change_priority(pid_t pid, int policy, int sched_policy);



#endif // PRIORITY_H

