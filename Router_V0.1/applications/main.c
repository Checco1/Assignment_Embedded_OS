#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#include "./Thread_Function/Dijkstra_example.h"
#include "./Thread_Function/Thread_1.h"

extern usr users[MAX_USER];
extern int g_matrix[MAX_USER][MAX_USER];
extern int active_user;
extern struct rt_semaphore matrix_sem;

static rt_thread_t tid1 = RT_NULL;

int main(void)
{

    rt_sem_init(&matrix_sem, "matrixsem", 1, RT_IPC_FLAG_FIFO);
    active_user = 10;
    rt_sem_release(&matrix_sem);

    tid1 = rt_thread_create("thread1",
            random_mixer, &matrix_sem, 512, 25, 5);
    rt_thread_startup(tid1);

    return 0;
}
