//scheduling.h
#ifndef SCHEDULING_H
#define SCHEDULING_H

#define CPU_HAS_NO_RUNNING_PROCESS (-1)
#define RR_TIME_SLICE_UNIT_TIME 500

#include "process.h" // struct process

struct Scheduler_State{
	int num_process;
	int scheduling_policy;

	int elapsed_unit_time; 
	int CPU_has_idx_running_process; 
	int num_finished_process; 
	int RR_time_slice_start_running_count_time; 
};


int CPU_has_running_process_or_not( const struct Scheduler_State* state_ptr );

int CPU_running_process_finished_or_not( struct process* process_input, const struct Scheduler_State* state_ptr );

void CPU_running_process_finished(struct process* process_input, struct Scheduler_State* state_ptr);

void check_process_ready_arrive(struct process* process_input, struct Scheduler_State* state_ptr, 
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[],
int call_child_fd[], int wait_child_fd[] );

void process_preempted_in(struct process* process_input, struct Scheduler_State* state_ptr, int idx_next_running_process,
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] );

void process_preempted_out(struct process* process_input, struct Scheduler_State* state_ptr,
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] );

int next_running_process(struct process* process_input, struct Scheduler_State* state_ptr, 
int* RR_ready_queue_front, int* RR_ready_queue_back, int RR_ready_queue[] );

int scheduler_finished_or_not( const struct Scheduler_State* state_ptr );

#endif // SCHEDULING_H
