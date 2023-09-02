#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#include "./Thread_Function/Dijkstra_example.h"
#include "./Thread_Function/Thread_1.h"
#include "./Thread_Function/wdg.h"

extern usr users[MAX_USER];
extern int g_matrix[MAX_USER][MAX_USER];
extern int active_user;
extern struct rt_semaphore matrix_sem;

static rt_thread_t tid1 = RT_NULL;

int main(void)
{

    /* Initialize WDG MONITOR Timer */
    rt_timer_init(&wdg_monitor_timer, "timer1",  /* Timer name is timer1 */
                    wdg_monitor_timeout, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_MONITOR_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */

    /* Start WDG MONITOR Timer */
    rt_timer_start(&wdg_monitor_timer);

    /* Initialize WDG Timer */
    rt_timer_init(&wdg_timer, "WATCHDOG_TIMER",  /* Timer name is timer1 */
                    wdg_timeout, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */


    /* Start WDG Timer */
    rt_timer_start(&wdg_timer);

    // Set to UNKNOWN all status flags
    STATUS_THREAD_1 = UNKNOWN;
    STATUS_THREAD_2 = UNKNOWN;

    rt_sem_init(&matrix_sem, "matrixsem", 1, RT_IPC_FLAG_FIFO);
    active_user = 10;
    rt_sem_release(&matrix_sem);

    tid1 = rt_thread_create("thread1",
            random_mixer, &matrix_sem, 512, 25, 5);
    rt_thread_startup(tid1);

    return 0;
}
