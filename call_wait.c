//call_wait.c

#include <unistd.h> // read() write()
#include <stdio.h> // fprintf(stderr)
#include "common.h" 

void call_child(int call_child_list){
	if( write(call_child_list, "p", 1) != 1)  ERROR_EXIT("call_child()error!\n");
}

void wait_child(int wait_child_list){
	char c;
	if( read(wait_child_list, &c, 1) != 1 )  ERROR_EXIT("wait_child() error!\n");
	if( c != 'c' )  ERROR_EXIT("wait_child() wrong input!\n");
}

void call_parent(int call_parent_list){
	if( write(call_parent_list, "c", 1) != 1 )  ERROR_EXIT("call_parent() error!\n");
}

void wait_parent(int wait_parent_list){
	char c;
	if( read(wait_parent_list, &c, 1) != 1 ){ERROR_EXIT("wait_parent()  error!\n");}
	if( c != 'p' )  ERROR_EXIT("wait_parent() wrong input!\n");
}

