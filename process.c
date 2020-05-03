//process.c
#define _GNU_SOURCE 

#include <unistd.h> // fork() pid_t getpid() pipe() close()
#include "process.h" 
#include <sched.h> // SCHED_RR
#include <stdio.h> // scanf() printf() fprintf(stderr, )
#include <sys/syscall.h> // syscall()
#include <stdlib.h> //  malloc()
#include <string.h> // strcmp()

#include "priority.h" // change_priority() choose_core() CHILD_CPU_CORE
#include "call_wait.h" // call_parent() wait_parent()
#include "common.h" //type and constant

struct process* init_scheduler(struct Scheduler_State* state_ptr){
	// 1. input
	char policy_string[ MAX_STRING_LEN ];
	if( scanf("%s", policy_string) != 1 )
	{
		ERROR_EXIT("Input file error\n");
	}

	if( strcmp(policy_string, "FIFO") == 0 )  state_ptr->policy = POLICY_FIFO;
	else if( strcmp(policy_string, "RR") == 0 )  state_ptr->policy = POLICY_RR;
	else if( strcmp(policy_string, "SJF") == 0 )  state_ptr->policy = POLICY_SJF;
	else if( strcmp(policy_string, "PSJF") == 0 )  state_ptr->policy = POLICY_PSJF;
	else  ERROR_EXIT("scheduling policy must be either of FIFO RR SJF or PSJF\n");

	//  number of processes
	if( scanf("%d", &(state_ptr->num_process)) != 1 )  ERROR_EXIT("scanf number of processes error.\n");

	struct process* process_input = (struct process*)malloc( (state_ptr->num_process) * sizeof(struct process) ); //TODO : free() in scheduler.c
	for(int i=0;i<(state_ptr->num_process);i++){
		if((scanf("%s%d%d", process_input[i].name, &process_input[i].ready_time, &process_input[i].execution_time ))<=0)	ERROR_EXIT("Input data wrong\n");
		process_input[i].input_order = i; //for calc.c
		process_input[i].IS_RUNNING = 0;
	}
	// 3. states that change with time
	state_ptr->past_time = 0;
	state_ptr->RUNNING_IDX = CPU_IS_FREE;
	state_ptr->num_finished_process = 0;

	// 4. set priority & affinity
	pid_t pid=getpid();
	choose_core(pid, SCHEDULER_CPU_CORE); 
	change_priority(pid, SCHED_FIFO, PRIORITY_HIGH); 

	return process_input;
}





pid_t process_execute(struct process process_input_1, int* call_child_list_ptr, int* wait_child_list_ptr ){  //TODO : struct process : call by value
	int P2C[2],C2P[2];// the pipe parent to child and the pipe child to parent 
	if( pipe(P2C) == (-1) )   // the [0] for reading and [1] For writing
		ERROR_EXIT("pipe P2C create error\n");
	if( pipe(C2P) == (-1) )  
		ERROR_EXIT("pipe C2P create error\n");
	
	pid_t fork_pid = fork();
	if(fork_pid < 0)  {
		ERROR_EXIT("fork() error in process_execute() !!\n");
	}
	else if(fork_pid == 0){	//In child process
		close(P2C[1]);		//child write and parent read
		close(C2P[0]);
		
		pid_t child_pid = getpid();
		
		choose_core(child_pid, CHILD_CPU_CORE);
		change_priority(child_pid, SCHED_RR, PRIORITY_HIGH); 
		
		printf("%s %d\n", process_input_1.name, child_pid);

		long start_time;
		for(int i=0;i<process_input_1.execution_time;i++){  
			wait_parent( P2C[0] ); 
			if(i == 0)  start_time = syscall(MY_GETTIME); 
			EXECUTE_UNIT_TIME; 
			call_parent( C2P[1] ); 

		}
		long end_time = syscall(MY_GETTIME); 
		syscall(MY_DPRINT, child_pid, start_time, end_time); 
#ifdef CALC
		fprintf(stderr, "[Project1] %d %ld.%09ld %ld.%09ld\n", process_input_1.input_order, start_time/GIGA, start_time%GIGA, end_time/GIGA, end_time%GIGA); 
#endif 
		exit(0);
	}

	close(P2C[0]);	//parent write child read
	close(C2P[1]);
	*call_child_list_ptr = P2C[1];
	*wait_child_list_ptr = C2P[0];
	
	return fork_pid; //TODO : modify struct process* process_input after return
}





