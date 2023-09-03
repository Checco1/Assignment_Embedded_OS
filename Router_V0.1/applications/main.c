#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#include "./Thread_Function/Dijkstra_example.h"
#include "./Thread_Function/Thread_1.h"
#include "./Thread_Function/Thread_2.h"
#include "./Thread_Function/wdg.h"

int main(void)
{

    // Set to UNKNOWN all status flags
    STATUS_THREAD_1 = UNKNOWN;
    STATUS_THREAD_2 = UNKNOWN;

    rt_sem_init(&matrix_sem, "matrixsem", 1, RT_IPC_FLAG_FIFO);
    active_user = 10;

    rt_thread_init(&thread1,
                   "thread1",
                   random_mixer,
                   &matrix_sem,
                   &thread1_stack[0],
                   sizeof(thread1_stack),
                   20, 5);

    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
                   "thread2",
                   solver,
                   &matrix_sem,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   20, 10);

    rt_thread_startup(&thread2);

    /* Initialize WDG Timer */
    rt_timer_init(&wdg_timer, "WATCHDOG_TIMER",  /* Timer name is timer1 */
                    wdg_timeout, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */

    /* Start WDG Timer */
    rt_timer_start(&wdg_timer);

    prova.thread1 = thread1;
    prova.thread2 = thread2;
    prova.wdg_timer = wdg_timer;
    /* Initialize WDG MONITOR Timer */
    rt_timer_init(&wdg_monitor_timer, "wdg_monitor_timer",  /* Timer name is timer1 */
                    wdg_monitor_timeout, /* Callback handler for timeout */
                    &prova, /* Entry parameter of the timeout function */
                    RT_TICK_PER_SECOND*WDG_MONITOR_TIME_SEC, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */

    /* Start WDG MONITOR Timer */
    rt_timer_start(&wdg_monitor_timer);



    return 0;
}
