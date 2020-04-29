//process.h
#ifndef PROCESS_H
#define PROCESS_H

#define SYS_PJ1_PRINT 335
#define SYS_PJ1_TIME 334
#define UNIT_TIME 1000000UL
#define NANOSECOND_BASE ((long)1000000000)

#define MAX_STRING_LEN 128 // for process name & scheduling policy

#define POLICY_FIFO 0
#define POLICY_RR 1
#define POLICY_SJF 2
#define POLICY_PSJF 3

//#include <unistd.h> // pid_t
#include <sys/types.h> // pid_t

struct process{
	char name[ MAX_STRING_LEN ];
	int ready_time;
	int execution_time;
	int input_order; 
	int TASK_RUNNING_or_not; 
	int pid; 
};

void execute_unit_time();
struct process* read_input(int* scheduling_policy_ptr, int* num_process_ptr);

pid_t ready_process_execute(struct process process_input_1, 
	int* call_child_fd_ptr, int* wait_child_fd_ptr );

#endif // PROCESS_H
