
#include <unistd.h> //for pipe read and write
#include "common.h" 
#include <stdio.h> 
#define ERR_EXIT 1 



void call_parent(int call_parent_fd)
{
	if( write(call_parent_fd, "c", 1) != 1 )  error_exit("call_parent() write() error!\n", ERR_EXIT);
}

void wait_parent(int wait_parent_fd)
{
	char c;

	if( read(wait_parent_fd, &c, 1) != 1 ){
		error_exit("wait_parent() read() error!\n", ERR_EXIT);}
	if( c != 'p' )  error_exit("wait_parent() wrong data!\n", ERR_EXIT);
}

void call_child(int call_child_fd)
{
	if( write(call_child_fd, "p", 1) != 1)  error_exit("call_child() write() error!\n", ERR_EXIT);
}

void wait_child(int wait_child_fd)
{
	char c;
	if( read(wait_child_fd, &c, 1) != 1 )  error_exit("wait_child() read() error!\n", ERR_EXIT);
	if( c != 'c' )  error_exit("wait_child() wrong data!\n", ERR_EXIT);
}