//scheduling.c

#define ERR_EXIT 1
#define _GNU_SOURCE 
#include <stdio.h> 
#include <sys/wait.h> 
#include "process.h" 
#include "priority.h" 
#include "scheduling.h" 
#include "rr_queue.h" 
#include "common.h" 

int CPU_has_running_process_or_not( const struct Scheduler_State* state_ptr ){
	return ( state_ptr->CPU_has_idx_running_process != CPU_HAS_NO_RUNNING_PROCESS );
}

void set_CPU_has_running_process( struct Scheduler_State* state_ptr, int CPU_has_idx_running_process ){
	state_ptr->CPU_has_idx_running_process = CPU_has_idx_running_process;
}

void set_CPU_has_no_running_process( struct Scheduler_State* state_ptr ){
	state_ptr->CPU_has_idx_running_process = CPU_HAS_NO_RUNNING_PROCESS;
}

int CPU_running_process_finished_or_not( struct process* process_input, const struct Scheduler_State* state_ptr ){
	return ( process_input[ state_ptr->CPU_has_idx_running_process ].execution_time == 0 );
}
void CPU_running_process_finished(struct process* process_input, struct Scheduler_State* state_ptr){


	(state_ptr->num_finished_process)++; 
	process_input[ state_ptr->CPU_has_idx_running_process ].TASK_RUNNING_or_not = 0;
	set_CPU_has_no_running_process( state_ptr );
	wait(NULL);
}
void check_process_ready_arrive(struct process* process_input, struct Scheduler_State* state_ptr, 
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[],
int call_child_fd[], int wait_child_fd[] ){
	for(int i=0;i<(state_ptr->num_process);i++){
		if( process_input[i].ready_time == state_ptr->elapsed_unit_time ){
			process_input[i].pid = ready_process_execute( process_input[i], 
				&call_child_fd[i], &wait_child_fd[i] );

			process_input[i].TASK_RUNNING_or_not = 1;

			if( state_ptr->scheduling_policy == POLICY_RR )
				push_queue(RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue, i ); 


		}
	}
}

void process_preempted_in(struct process* process_input, struct Scheduler_State* state_ptr, int idx_next_running_process,
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] ){


	set_CPU_has_running_process( state_ptr, idx_next_running_process );


	if( state_ptr->scheduling_policy == POLICY_RR ){
		state_ptr->RR_time_slice_start_running_count_time = 0;
		pop_queue(RR_ready_queue_front, RR_ready_queue_back);
	}
}

void process_preempted_out(struct process* process_input, struct Scheduler_State* state_ptr,
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] ){


	if( state_ptr->scheduling_policy == POLICY_RR )
		push_queue(RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue, state_ptr->CPU_has_idx_running_process ); 

}


int next_running_process(struct process* process_input, struct Scheduler_State* state_ptr, 
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] ){

	if( (state_ptr->scheduling_policy == POLICY_FIFO || state_ptr->scheduling_policy == POLICY_SJF) && CPU_has_running_process_or_not( state_ptr ) )
		return state_ptr->CPU_has_idx_running_process; 

	int idx_next_running_process = CPU_HAS_NO_RUNNING_PROCESS; 

	if( state_ptr->scheduling_policy == POLICY_SJF || state_ptr->scheduling_policy == POLICY_PSJF ){
		
		for(int i=0;i<(state_ptr->num_process);i++){
			if( process_input[i].TASK_RUNNING_or_not == 0 )  continue;
			if( idx_next_running_process == CPU_HAS_NO_RUNNING_PROCESS )
				idx_next_running_process = i;
			if( process_input[i].execution_time < process_input[ idx_next_running_process ].execution_time ) 
				idx_next_running_process = i;
		}

	}else if( state_ptr->scheduling_policy == POLICY_FIFO ){

		for(int i=0;i<(state_ptr->num_process);i++){
			if( process_input[i].TASK_RUNNING_or_not == 1 ){
				idx_next_running_process = i;  break; //for i
			}
		}
//Round-Robin
	}else{ 

		if( queue_empty(RR_ready_queue_front, RR_ready_queue_back) ){
			idx_next_running_process = state_ptr->CPU_has_idx_running_process; // same process keeps running
			if( state_ptr->RR_time_slice_start_running_count_time == RR_TIME_SLICE_UNIT_TIME )
				state_ptr->RR_time_slice_start_running_count_time = 0;
		}else if( !CPU_has_running_process_or_not( state_ptr ) ){
			idx_next_running_process = queue_front(RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue);
		}else if( state_ptr->RR_time_slice_start_running_count_time == RR_TIME_SLICE_UNIT_TIME ){ 
			idx_next_running_process = queue_front(RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue);
		}else{ 
			idx_next_running_process = state_ptr->CPU_has_idx_running_process; // same process keeps running
		}


	}
	
	return idx_next_running_process;
}
int scheduler_finished_or_not( const struct Scheduler_State* state_ptr ){
	return ( state_ptr->num_finished_process == state_ptr->num_process );
}
