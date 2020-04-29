
#include <stdio.h> 
#include <stdlib.h> 

void err_exit(char* mesg, int ERR_EXIT){
	perror(mesg); 	
	if( ERR_EXIT == 1)  exit(1);
	else  return;
}


