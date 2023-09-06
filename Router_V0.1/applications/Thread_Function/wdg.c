#include "Thread_Function/wdg.h"
#include <rthw.h>



void monitor_timeout()
{
    extern rt_flag_t STATUS_THREAD_1;
    extern rt_flag_t STATUS_THREAD_2;
    extern rt_thread_t thread1;
    extern rt_thread_t thread2;
    extern struct rt_timer wdg_timer;

    rt_tick_t clock_tick;
    clock_tick = rt_tick_get();
    rt_kprintf("MONITOR Timer check at %d clock tick\n", clock_tick);

    if (STATUS_THREAD_1 == ALIVE && STATUS_THREAD_2 == ALIVE) {

        rt_tick_t resetTimeValue;
        resetTimeValue = RT_TICK_PER_SECOND*WDG_TIME_SEC;

        rt_timer_stop(&wdg_timer);
        rt_timer_control(&wdg_timer, RT_TIMER_CTRL_SET_TIME, &resetTimeValue);
        rt_timer_start(&wdg_timer);

        // Set status flags to UNKNOWN
        STATUS_THREAD_1 = UNKNOWN;
        STATUS_THREAD_2 = UNKNOWN;

    } else {

        if (STATUS_THREAD_1 == UNKNOWN){
            rt_thread_delete(thread1);
            STATUS_THREAD_1 = DEAD;
            rt_kprintf("\n");
            rt_kprintf("INFO:   THREAD 1 does not respond! \n");
            rt_kprintf("    STATUS_THREAD_1 is %d\n", STATUS_THREAD_1);
            rt_kprintf("    THREAD 1 killed\n");
            rt_kprintf("\n");
            return;
        }

        if (STATUS_THREAD_2 == UNKNOWN){
            rt_thread_delete(thread2);
            STATUS_THREAD_2 = DEAD;
            rt_kprintf("\n");
            rt_kprintf("INFO:   THREAD 2 does not respond! \n");
            rt_kprintf("    STATUS_THREAD_2 is %d\n", STATUS_THREAD_2);
            rt_kprintf("    THREAD 2 killed\n");
            rt_kprintf("\n");
            return;
        }
    }
}

void wdg_timeout()
{
    rt_kprintf("WATCHDOG is called, time is up!!! \n");
    rt_kprintf("The killed threads should be recreated!!! \n");
}
