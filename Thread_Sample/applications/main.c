#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>

#define THREAD_PRIORITY         25   //uint8_t
#define THREAD_STACK_SIZE       512 //Grandezza Stack in Byte
#define THREAD_TIMESLICE        5   //Tempo in tick

static rt_thread_t tid1 = RT_NULL;
static void thread1_entry(void *parameter);

int main(void)
{
    /* Creat thread 1, Name is thread1，Entry is thread1_entry */
    tid1 = rt_thread_create("thread1",
                            (void *)thread1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* Start this thread if you get the thread control block */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

static void thread1_entry(void *parameter)
{
        rt_kprintf("thread1 \n");
}


/*Crea un comando da utilizzare nella shell*/
MSH_CMD_EXPORT(main/*Nome Comando*/,Start Thread_sample/*Descrizione*/);
