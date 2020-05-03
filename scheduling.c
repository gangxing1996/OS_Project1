//scheduling.c
#define _GNU_SOURCE //XXX 
#include <stdio.h> // scanf() fprintf(stderr)
#include <sys/wait.h> // wait()
#include "process.h" // process_execute()  
#include "priority.h" // CPU_preempt_in() CPU_preempt_out()
#include "rr_queue.h" 
#include "call_wait.h" // call_child() wait_child()
#include "common.h" 

#define CPU_IS_RUNNING(state)	(state->RUNNING_IDX != -1)


void CPU_running_process_finished(struct process* process_input, struct Scheduler_State* state_ptr)
	{
	(state_ptr->num_finished_process)++; 
	process_input[ state_ptr->RUNNING_IDX ].IS_RUNNING = 0;
	state_ptr->RUNNING_IDX = CPU_IS_FREE;
	wait(NULL);
	}

void check_process_ready(struct process* process_input, struct Scheduler_State* state_ptr, int* queue_front, int* queue_back, int queue[],int call_child_list[], int wait_child_list[] ){
	for(int i=0;i<(state_ptr->num_process);i++){
		if( process_input[i].ready_time == state_ptr->past_time ){
			process_input[i].pid = process_execute( process_input[i], &call_child_list[i], &wait_child_list[i] );

			process_input[i].IS_RUNNING = 1;

			if( state_ptr->policy == POLICY_RR )
				push_queue( queue_back, queue, i ); 


		}
	}
}

void preempted_in(struct process* process_input, struct Scheduler_State* state_ptr, int next_process,int* queue_front, int* queue_back, int queue[] )
{
	state_ptr->RUNNING_IDX = next_process;//change to next progress
	if( state_ptr->policy == POLICY_RR ){
		state_ptr->rr_timer = 0;
		pop_queue(queue_front, queue_back); 
	}
}

void preempted_out(struct process* process_input, struct Scheduler_State* state_ptr,int* queue_front, int* queue_back, int queue[] )
{
	if( state_ptr->policy == POLICY_RR ) push_queue( queue_back, queue, state_ptr->RUNNING_IDX ); 
}


int choose_next(struct process* process_input, struct Scheduler_State* state_ptr, int* queue_front, int* queue_back, int queue[] ){


	if( (state_ptr->policy == POLICY_FIFO || state_ptr->policy == POLICY_SJF) && CPU_IS_RUNNING( state_ptr ) )
		return state_ptr->RUNNING_IDX; // continue to run

	int next_process = CPU_IS_FREE; //init

	if( state_ptr->policy == POLICY_SJF || state_ptr->policy == POLICY_PSJF ){
		
		for(int i=0;i<(state_ptr->num_process);i++){
			//to find the small excution time with small order
			if( process_input[i].IS_RUNNING == 0 )  continue;
			if( next_process == CPU_IS_FREE )
				next_process = i;
			if( process_input[i].execution_time < process_input[ next_process ].execution_time ) 
			{ 
				next_process = i;
			}
			//choose the smaller order
				
		}

	}else if( state_ptr->policy == POLICY_FIFO ){
		


		for(int i=0;i<(state_ptr->num_process);i++){
			if( process_input[i].IS_RUNNING == 1 )//according to ready time
			{
				next_process = i;  
				break; 
			}
		}
//-----------------for RR--------
	}else{ 

		if( *queue_front >= *queue_back )//empty not change
		{
			next_process = state_ptr->RUNNING_IDX; 
			if( state_ptr->rr_timer == QUANTUM_TIME )
				state_ptr->rr_timer = 0;// reset timer, if to Qtime
		}
		else if( !CPU_IS_RUNNING( state_ptr ) )
		{
			//CPU is free 
			next_process = queue[ (*queue_front) ];
		}
		else if( state_ptr->rr_timer == QUANTUM_TIME )
		{ 
			next_process = queue[ (*queue_front) ];
		}
		else
		{ 
			next_process = state_ptr->RUNNING_IDX; // still have time
		}
	}
	
	return next_process;
}




void schedule_parent(struct Scheduler_State* state,struct process* process_input,int call_child_list[],int wait_child_list[],int* queue_front,int* queue_back,int queue[]){
	while(1){
		
		if( CPU_IS_RUNNING( state ) ){
			if( process_input[ state->RUNNING_IDX ].execution_time == 0 )
			{
				CPU_running_process_finished( process_input,state );//check the running is finished or not
				if(  state->num_finished_process == state->num_process  )  break; // done and out
			}
		}


		check_process_ready( process_input, state , queue_front, queue_back, queue, call_child_list, wait_child_list ); 

		int next_process = choose_next( process_input, state , queue_front, queue_back, queue);
		// choose next to change process to ex	
		if( next_process != CPU_IS_FREE ){
			call_child( call_child_list[ next_process ] );

			if( CPU_IS_RUNNING( state ) ){
				if( state->RUNNING_IDX != next_process ){
					
					preempted_out( process_input, state ,queue_front, queue_back, queue);
					preempted_in( process_input, state, next_process , queue_front, queue_back, queue);

				}
			}else{  //if cpu is free
				preempted_in( process_input, state, next_process ,queue_front, queue_back, queue); 
			}
		}
		
		//set counter for a UNIT TIME
		if( CPU_IS_RUNNING( state ) ){
			process_input[ state->RUNNING_IDX ].execution_time--;
			
		if( state->policy == POLICY_RR )  
			state->rr_timer++; 
		}
		
		state->past_time++;
		EXECUTE_UNIT_TIME;
		
		if( next_process != CPU_IS_FREE ){
			//wait child to run
			wait_child( wait_child_list[ next_process ] ); 
		}
	}
}
