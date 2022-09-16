/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020/12/31     Bernard      Add license info
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <ulog.h>

#define THREAD_STCKSIZE 512
#define THREAD_PRIOPITY 20
#define THREAD_TIMESLICE 20

static rt_thread_t tid1;
static rt_thread_t tid2;
extern rt_sem_t sem1;
extern rt_mq_t mq1;
extern rt_uint32_t task2_num;

void num_to_week(int num)
{
    switch (num) {
        case 0: LOG_D("Sunday");
        break;
        case 1: LOG_D("Monday");
        break;
        case 2: LOG_D("Tuesday");
        break;
        case 3: LOG_D("Wednesday");
        break;
        case 4: LOG_D("Thursday");
        break;
        case 5: LOG_D("Friday");
        break;
        case 6: LOG_D("Thursday");
        break;
        case 7: LOG_D("Saturday");
        break;
    }
}

void task1_entry(void *parameter)
{
    int buf[1] = {0};
    while(1) {
        if (rt_mq_recv(mq1, buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK) {
            num_to_week(*buf);
        }
    }
}

void task2_entry(void *parameter)
{
    int result;

    while(1) {
        result = rt_sem_take(sem1, 1000);

        if(result == RT_EOK) {
            num_to_week(task2_num);
            task2_num = -1;
        }
    }
}

int main(int argc, char *argv[])
{
    printf("\nHello RT-Thread!\n");

    if(ulog_init() < 0)
        return -1;

    mq1 = rt_mq_create("mq1", 20, 1, RT_IPC_FLAG_PRIO);
    sem1 = rt_sem_create("sem1", 0, RT_IPC_FLAG_PRIO);

    if (mq1 == RT_NULL)
    {
        rt_kprintf("create mq1 failed.\n");
        return -1;
    }

    tid1 = rt_thread_create("th_task1", task1_entry, RT_NULL, THREAD_STCKSIZE, THREAD_PRIOPITY, THREAD_TIMESLICE);

    if (tid1 != RT_NULL) {
        rt_thread_startup(tid1);
    }

    tid2 = rt_thread_create("th_task2", task2_entry, RT_NULL, THREAD_STCKSIZE, THREAD_PRIOPITY - 1, THREAD_TIMESLICE);

    if (tid2 != RT_NULL) {
        rt_thread_startup(tid2);
    }

    return 0;
}
