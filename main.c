#include <stdlib.h> 
#include "scheduling.h" 
#include "priority.h" 
#include <sched.h> 
#include <unistd.h> 
#include "process.h" 
#include "rr_queue.h" 
#include <stdio.h>
#include "call_wait.h" 

struct process* init_scheduler(struct Scheduler_State* state_ptr);
int main(int argc, char* argv[]){
	struct Scheduler_State state;
	struct process* process_input = init_scheduler( &state );

	int call_child_fd[ state.num_process ],wait_child_fd[ state.num_process ];
	int* RR_ready_queue_front = (int*)malloc(sizeof(int));
	int* RR_ready_queue_back = (int*)malloc(sizeof(int));
	int* RR_ready_queue = init_queue(RR_ready_queue_front, RR_ready_queue_back);

	while(1){
		if( CPU_has_running_process_or_not( &state ) ){
			if( CPU_running_process_finished_or_not( process_input, &state ) ){
				CPU_running_process_finished( process_input, &state );
				if( scheduler_finished_or_not( &state ) )  break; 
			}
		}
		check_process_ready_arrive( process_input, &state , RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue, call_child_fd, wait_child_fd ); 

		int idx_next_running_process = next_running_process( process_input, &state , 
			RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue);
			
		if( idx_next_running_process != CPU_HAS_NO_RUNNING_PROCESS ){
			call_child( call_child_fd[ idx_next_running_process ] ); 

			if( CPU_has_running_process_or_not( &state ) ){
				if( state.CPU_has_idx_running_process != idx_next_running_process ){
					process_preempted_out( process_input, &state ,RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue);
					process_preempted_in( process_input, &state, idx_next_running_process , RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue);
				}
			}else{  
				process_preempted_in( process_input, &state, idx_next_running_process ,RR_ready_queue_front, RR_ready_queue_back, RR_ready_queue); 
			}
		}
		if( CPU_has_running_process_or_not( &state ) ){
			process_input[ state.CPU_has_idx_running_process ].execution_time--;
			
			if( state.scheduling_policy == POLICY_RR )  
				state.RR_time_slice_start_running_count_time++; 
		}
		state.elapsed_unit_time++;
		execute_unit_time();
		if( idx_next_running_process != CPU_HAS_NO_RUNNING_PROCESS ){
			wait_child( wait_child_fd[ idx_next_running_process ] ); //TODO
		}
	}
	free( process_input );
	free( RR_ready_queue );
	return 0;
}
struct process* init_scheduler(struct Scheduler_State* state_ptr){

	struct process* process_input = read_input( &(state_ptr->scheduling_policy), &(state_ptr->num_process) );

	state_ptr->elapsed_unit_time = 0;
	state_ptr->CPU_has_idx_running_process = CPU_HAS_NO_RUNNING_PROCESS;
	state_ptr->num_finished_process = 0;

	use_CPU_core(getpid(), SCHEDULER_CPU); 
	set_priority(getpid(), SCHED_FIFO, PRIORITY_HIGH); 
	return process_input;
}