//priority.h
#ifndef PRIORITY_H
#define PRIORITY_H
#include <sys/types.h> 


#define PRIORITY_INIT (int)50
#define PRIORITY_LOW (int)10
#define PRIORITY_HIGH (int)80

#define SCHEDULER_CPU 0
#define CHILD_CPU 1


void use_CPU_core(pid_t pid, int idx_core);
void set_priority(pid_t pid, int policy, int sched_policy);

#endif // PRIORITY_H

