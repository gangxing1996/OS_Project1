//syscall 335

#include <linux/linkage.h>
#include <linux/kernel.h>


asmlinkage void sys_my_dprint(int pid,long start_time, long end_time)
{   
    static const long giga = 1000000000;
    printk("[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start_time / giga, start_time % giga, end_time / giga, end_time % giga);
    return;
}