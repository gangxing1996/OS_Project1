//scheduler.c

#include <stdlib.h> //  free() malloc()
#include <stdio.h> // fprintf(stderr)
#include <sched.h> // SCHED_FIFO
#include <unistd.h> // getpid()

#include "process.h" // process 
#include "rr_queue.h" 
#include "scheduling.h" //  Scheduler_State 
#include "common.h"


int main(int argc, char* argv[]){
	struct Scheduler_State state;
	struct process* process_input = init_scheduler( &state );

	int call_child_list[ state.num_process ];
	int wait_child_list[ state.num_process ];


	int* queue_front = (int*)malloc(sizeof(int));	*queue_front=0;
	int* queue_back = (int*)malloc(sizeof(int)); 	*queue_back=0;
	
	int* queue = (int*)malloc( MAX_QUEUE_SIZE * sizeof(int) ); 


	schedule_parent(&state,process_input,call_child_list,wait_child_list,queue_front,queue_back,queue);


    free( process_input );          
	free( queue );
	free(queue_front);                     
	free(queue_back);            
	return 0;

	
}
