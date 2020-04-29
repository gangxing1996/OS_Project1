//call_wait.h
#ifndef CALL_WAIT_H
#define CALL_WAIT_H

void call_child(int call_child_fd);
void wait_child(int wait_child_fd);

void call_parent(int call_parent_fd);
void wait_parent(int wait_parent_fd);

#endif // call_wait_H
