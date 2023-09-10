#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#include "./Thread_Function/Dijkstra_example.h"
#include "./Thread_Function/Thread_1.h"
#include "./Thread_Function/Thread_2.h"
#include "./Thread_Function/wdg.h"
#include "./Thread_Function/uart.h"

int main(void)
{
    extern rt_thread_t thread1;
    extern rt_thread_t thread2;
    extern struct rt_semaphore matrix_sem;
    extern struct rt_semaphore users_sem;
    extern struct rt_timer monitor_timer;
    extern struct rt_timer wdg_timer;
    extern int g_matrix[MAX_USER][MAX_USER];

    extern usr users[MAX_USER];

    for (int i = 0; i < MAX_USER; ++i) {
        for (int j = 0; j < MAX_USER; ++j) {
            if (i == j) {
                users[i].distance[j] = 0;
            } else {
                users[i].distance[j] = -1;
            }
        }
        users[i].isActive = FALSE;
        users[i].name[0] = '\0';
    }

    for (int i = 0; i < MAX_USER; ++i) {
        for (int j = 0; j < MAX_USER; ++j) {
            g_matrix[i][j] = 0;
        }
    }

    rt_sem_init(&matrix_sem, "matrixsem", 1, RT_IPC_FLAG_FIFO);
    rt_sem_init(&users_sem, "usersem", 1, RT_IPC_FLAG_FIFO);

    active_user = 4;

    strcpy(users[0].name ,"Francesco_Calice");
    users[0].isActive = TRUE;

    strcpy(users[1].name,"Francesco_Spagnoletti");
    users[1].isActive = TRUE;

    strcpy(users[2].name,"Lucia_Vencato");
    users[2].isActive = TRUE;

    strcpy(users[3].name,"Stefano_Di_Carlo");
    users[3].isActive = TRUE;

    thread1 = rt_thread_create("thread1",
                            random_mixer, RT_NULL,
                            STACK_SIZE_T1,
                            PRIORITY_T1, RT_TICK_PER_SECOND*THREAD_1_SLICE_TIME);

    rt_thread_startup(thread1);
    STATUS_THREAD_1 = ALIVE;

    thread2 = rt_thread_create("thread2",
                            solver, RT_NULL,
                            STACK_SIZE_T2,
                            PRIORITY_T2, RT_TICK_PER_SECOND*THREAD_2_SLICE_TIME);

    rt_thread_startup(thread2);
    STATUS_THREAD_2 = ALIVE;

    /* Initialize WDG MONITOR Timer */
    rt_timer_init(&monitor_timer, "wdg_monitor_timer",  /* Timer name is timer1 */
                    monitor_timeout, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_MONITOR_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */

    /* Start WDG MONITOR Timer */
    rt_timer_start(&monitor_timer);

    /* Initialize WDG MONITOR Timer */
    rt_timer_init(&wdg_timer, "wdg_monitor_timer",  /* Timer name is timer1 */
                    wdg_timeout, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */

    /* Start WDG MONITOR Timer */
    rt_timer_start(&wdg_timer);

    return 0;
}
