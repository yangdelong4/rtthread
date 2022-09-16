#include <rtthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

rt_mq_t mq1;

int task1(int argc, char **argv)
{
    int result;
    int number[1];

    if (argc != 2) {
        rt_kprintf("add parameter.\n");
        return -1;
    }

    number[0] = atoi(argv[1]);

    if (number[0] >= 7 || number[0] < 0) {
        rt_kprintf("argv2 : %d\n", number[0]);
        rt_kprintf("please input 0-7.\n");
        return -1;
    }

    result = rt_mq_send(mq1, (void *)number, sizeof(number[0]));

    if (result != RT_EOK)
    {
        rt_kprintf("send message queue failed.\n");
        return -1;
    }

    return 1;
}

MSH_CMD_EXPORT(task1, task1);