//rr_queue.h
#ifndef RR_QUEUE_H
#define RR_QUEUE_H
#define MAX_SIZE 1000007
int* init_queue(int* queue_front, int* queue_back);
void push_queue(int* queue_front, int* queue_back, int queue[], int x);
int queue_empty(int* queue_front, int* queue_back);
void pop_queue(int* queue_front, int* queue_back);
int queue_front(int* queue_front, int* queue_back, int queue[]);
int queue_size(int* queue_front, int* queue_back);
#endif 

