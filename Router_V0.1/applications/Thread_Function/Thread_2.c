#include "Thread_Function/Thread_2.h"

void solver(){

    extern usr users[MAX_USER];
    extern int g_matrix[MAX_USER][MAX_USER];
    extern int active_user;
    extern struct rt_semaphore matrix_sem;

    rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);
    for(int i = 0; i < active_user; i++){
        dijkstra(g_matrix, i);

    }
    rt_sem_release(&matrix_sem);
    STATUS_THREAD_2 = ALIVE;
    rt_thread_delay(RT_TICK_PER_SECOND*5);
    while(1){

    }
}

MSH_CMD_EXPORT(solver, Solve_Matrix);
