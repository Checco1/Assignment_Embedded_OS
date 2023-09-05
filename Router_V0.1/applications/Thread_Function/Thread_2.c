#include "Thread_Function/Thread_2.h"

void solver(){

    extern int g_matrix[MAX_USER][MAX_USER];
    extern int active_user;
    extern struct rt_semaphore matrix_sem;
    extern rt_flag_t STATUS_THREAD_2;

    while(1){

        rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);

        for(int i = 0; i < active_user; i++){
            dijkstra(g_matrix, i);
        }

        rt_sem_release(&matrix_sem);

        STATUS_THREAD_2 = ALIVE;
        rt_thread_delay(RT_TICK_PER_SECOND*15);
    }
}
