#include <rtthread.h>
#include <stdio.h>
#include <libc.h>
#include <stdlib.h>

rt_sem_t sem1;
rt_uint32_t task2_num = -1;

int task2(int argc, char **argv)
{
    if (argc != 2) {
        rt_kprintf("the number of parameter is wrong.\n");
        return -1;
    }

    task2_num = atoi(argv[1]);

    if (task2_num >= 0 && task2_num < 7)
        rt_sem_release(sem1);
    else
        rt_kprintf("please input 0-7.\n");

    return 0;
}

MSH_CMD_EXPORT(task2, task2);