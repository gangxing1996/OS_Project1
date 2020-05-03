//rr_queue.c

#include <stdlib.h> // malloc()
#include <stdio.h> // fprintf(stderr)
#include "common.h"



void push_queue( int* queue_back, int queue[], int input){
	queue[ (*queue_back) ] =input;
	(*queue_back)++;
	return;
}


void pop_queue(int* queue_front, int* queue_back){
	if(  *queue_front < *queue_back ){
		(*queue_front)++;
	}
	return;
}



