//process.h
#ifndef PROCESS_H
#define PROCESS_H



#include <sys/types.h> // pid_t
#include "common.h"


struct process* read_input(int* policy_ptr, int* num_process_ptr);

pid_t process_execute(struct process process_input_1, int* call_child_list_ptr, int* wait_child_list_ptr );

struct process* init_scheduler(struct Scheduler_State* state_ptr);


#endif // PROCESS_H
