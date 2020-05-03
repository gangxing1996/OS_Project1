//priority.c
#define _GNU_SOURCE //XXX


#include <unistd.h> // pid_t
#include <sched.h> // sched_setaffinity() sched_setscheduler()

#include "common.h" 

void choose_core(pid_t pid, int idx_core){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(idx_core, &mask);
	if( sched_setaffinity(pid, sizeof(mask), &mask) != 0)  ERROR_EXIT("sched_setaffinity() error in choose_core() !!\n");
	return;
}

void change_priority(pid_t pid, int policy, int sched_priority){
	struct sched_param param;
	param.sched_priority = sched_priority;
	if( sched_setscheduler(pid, policy, &param) != 0)  ERROR_EXIT("sched_set_scheduler() error in change_priority() !!\n");
	return;
}
