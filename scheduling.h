//scheduling.h
#ifndef SCHEDULING_H
#define SCHEDULING_H


void CPU_running_process_finished(struct process* process_input, struct Scheduler_State* state_ptr);

void check_process_ready(struct process* process_input, struct Scheduler_State* state_ptr, int* queue_front, int* queue_back, int queue[],int call_child_list[], int wait_child_list[] );

void preempted_in(struct process* process_input, struct Scheduler_State* state_ptr, int idx_next_running_process,int* queue_front, int* queue_back, int queue[] );

void preempted_out(struct process* process_input, struct Scheduler_State* state_ptr,int* queue_front, int* queue_back, int queue[] );

int choose_next(struct process* process_input, struct Scheduler_State* state_ptr, int* queue_front, int* queue_back, int queue[] );

void schedule_parent(struct Scheduler_State* state,struct process* process_input,int call_child_list[],int wait_child_list[],int* queue_front,int* queue_back,int queue[]);

#endif // SCHEDULING_H
