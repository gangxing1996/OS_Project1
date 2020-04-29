//process.c
#define _GNU_SOURCE 
#define ERR_EXIT 1
#include <unistd.h> 
#include "process.h" 
#include "common.h" 
#include "priority.h" 
#include <sched.h> 
#include <stdio.h> 
#include <sys/syscall.h> 
#include <stdlib.h> 
#include <string.h> 
#include "call_wait.h" 
void execute_unit_time(){
	volatile unsigned long i;
	for(i=0;i<UNIT_TIME;i++);
}
struct process* read_input(int* scheduling_policy_ptr, int* num_process_ptr){
	char scheduling_policy_string[ MAX_STRING_LEN ];
	if( scanf("%s", scheduling_policy_string) != 1 )  error_exit("First argument shall be the scheduling policy !!\n", ERR_EXIT);
	if( strcmp(scheduling_policy_string, "FIFO") == 0 )  *scheduling_policy_ptr = POLICY_FIFO;
	else if( strcmp(scheduling_policy_string, "RR") == 0 )  *scheduling_policy_ptr = POLICY_RR;
	else if( strcmp(scheduling_policy_string, "SJF") == 0 )  *scheduling_policy_ptr = POLICY_SJF;
	else if( strcmp(scheduling_policy_string, "PSJF") == 0 )  *scheduling_policy_ptr = POLICY_PSJF;
	else  error_exit("No such scheduling policy !!\n", ERR_EXIT);

	if( scanf("%d", num_process_ptr) != 1 )  error_exit("scanf number of processes error.\n", ERR_EXIT);

	struct process* process_input = (struct process*)malloc( (*num_process_ptr) * sizeof(struct process) ); 
	int tmp;
	for(int i=0;i<(*num_process_ptr);i++){
		tmp = scanf("%s%d%d", process_input[i].name, &process_input[i].ready_time, &process_input[i].execution_time );
		process_input[i].input_order = i; 
		process_input[i].TASK_RUNNING_or_not = 0;
	}
	return process_input;
}
pid_t ready_process_execute(struct process process_input_1, 
int* call_child_fd_ptr, int* wait_child_fd_ptr ){  
	int parent_to_child[2];
	int child_to_parent[2];
	if( pipe(parent_to_child) == (-1) )  error_exit("pipe() error!\n", ERR_EXIT);
	if( pipe(child_to_parent) == (-1) )  error_exit("pipe() error!\n", ERR_EXIT);
	
	pid_t fork_pid = fork();
	if(fork_pid < 0)  error_exit("fork() error in ready_process_execute() !!\n", ERR_EXIT);
	else if(fork_pid == 0){	
		close(parent_to_child[1]);
		close(child_to_parent[0]);
		
		pid_t child_pid = getpid();
		
		use_CPU_core(child_pid, CHILD_CPU);
		set_priority(child_pid, SCHED_RR, PRIORITY_HIGH); 
		
		printf("%s %d\n", process_input_1.name, child_pid);
		long start_time;
		for(int i=0;i<process_input_1.execution_time;i++){  
			wait_parent( parent_to_child[0] ); 
			if(i == 0)  start_time = syscall(SYS_PJ1_TIME); 
			execute_unit_time(); 
			call_parent( child_to_parent[1] ); 
		}
		long end_time = syscall(SYS_PJ1_TIME); 
		syscall(SYS_PJ1_PRINT, child_pid, start_time, end_time);

		exit(0);
	}
	close(parent_to_child[0]);
	close(child_to_parent[1]);
	*call_child_fd_ptr = parent_to_child[1];
	*wait_child_fd_ptr = child_to_parent[0];
	
	return fork_pid; 
}



