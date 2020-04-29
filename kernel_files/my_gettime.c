//syscall 334
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>
//my get time for operating system
//reference: https://www.codenong.com/4655711/
//remade by Zhengjiageng

asmlinkage long sys_my_gettime(void)
{ 
    static const long giga = 1000000000;
    struct timespec _t;  
    getnstimeofday(&_t);  
    return   (_t.tv_sec*giga + _t.tv_nsec);
}