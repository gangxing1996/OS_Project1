//common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h> // perror()
#include <stdlib.h> // exit()



//#define CALC

#define PRIORITY_INIT (int)50
#define PRIORITY_LOW (int)10
#define PRIORITY_HIGH (int)80

#define SCHEDULER_CPU_CORE 0
#define CHILD_CPU_CORE 1

#define MY_DPRINT 335
#define MY_GETTIME 334
#define UNIT_TIME 1000000UL
#define GIGA ((long)1000000000)

#define MAX_STRING_LEN 128 // for process name & scheduling policy

#define POLICY_FIFO     0
#define POLICY_RR       1
#define POLICY_SJF      2
#define POLICY_PSJF     3

#define CPU_IS_FREE (-1)
#define QUANTUM_TIME 500

#define MAX_QUEUE_SIZE 1000007



#define ERROR_EXIT(a) do {			\
	perror(a);                      \
    exit(1);			            \
    } while (0)

#define EXECUTE_UNIT_TIME do {      \
    volatile unsigned long i;       \
    for (i=0;i<UNIT_TIME;i++);      \
    } while (0)




struct process{
	char name[ MAX_STRING_LEN ];// eg. P1 P2 P3 ...
	int ready_time,execution_time,
	IS_RUNNING, 
	pid; // for scheduling.c : preempt_in() preempt_out()
    
    int input_order; //for calc.c
};

struct Scheduler_State{
	int num_process;
	int policy;
	int past_time; // the timeline from start
	int RUNNING_IDX; // index of running process in CPU, (-1) means that CPU has no running process
	int num_finished_process; // number of finished processes
	int rr_timer; //check the running time and quantum time
};


#endif // COMMON_H
