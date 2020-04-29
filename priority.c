#define _GNU_SOURCE 
#include <unistd.h> 
#include <sched.h> 
#include "common.h" 
#include "priority.h" 
#define ERR_EXIT 1

void use_CPU_core(pid_t pid, int idx_core){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(idx_core, &mask);
	if( sched_setaffinity(pid, sizeof(mask), &mask) != 0)  err_exit("sched_setaffinity() error !!\n", ERR_EXIT);
	return;
}

void set_priority(pid_t pid, int policy, int sched_priority){
	struct sched_param param;
	param.sched_priority = sched_priority;
	if( sched_setscheduler(pid, policy, &param) != 0)  err_exit("sched_set_scheduler() error !!\n", ERR_EXIT);
	return;
}
