//rr_queue.c

#include <stdlib.h> // malloc()
#include "rr_queue.h" // MAX_SIZE
#include <stdio.h> 

int* init_queue(int* q_front, int* q_end){
	int* queue = (int*)malloc( MAX_SIZE * sizeof(int) ); 
	*q_front = 0;
	*q_end = 0;
	return queue;
}

void push_queue(int* q_front, int* q_end, int queue[], int temp){
	queue[ (*q_end) ] = temp ;
	(*q_end)++;
}

int queue_empty(int* q_front, int* q_end){
	if( *q_front < *q_end )  return 0;
	else  return 1;
}

void pop_queue(int* q_front, int* q_end){
	if( queue_empty(q_front, q_end) == 0 )
	{
		(*q_front)++;
	}
	else  
		return;
}

int queue_front(int* q_front, int* q_end, int queue[]){
	return queue[ (*q_front) ];
}

int queue_size(int* q_front, int* q_end){
	return (*q_end) - (*q_front);
}


