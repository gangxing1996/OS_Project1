
#include <stdio.h> 
#include <stdlib.h> 

void error_exit(char* mesg, int err_exit){
	perror(mesg); 	
	if( err_exit == 1)  exit(1);
	else  return;
}


