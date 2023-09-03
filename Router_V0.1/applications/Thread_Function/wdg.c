#include "Thread_Function/wdg.h"



void wdg_monitor_timeout(struct parameters_timer_monitor_wdg *prova)
{
    rt_tick_t clock_tick;
    clock_tick = rt_tick_get();
    rt_kprintf("WHATCHDOG Timer check at %d clock tick\n", clock_tick);

    if (STATUS_THREAD_1 == ALIVE && STATUS_THREAD_2 == ALIVE) {

        rt_tick_t resetTimeValue;
        resetTimeValue = RT_TICK_PER_SECOND*WDG_TIME_SEC;

        rt_timer_stop(&(prova->wdg_timer));
        rt_timer_control(&(prova->wdg_timer), RT_TIMER_CTRL_SET_TIME, &resetTimeValue);
        rt_timer_start(&(prova->wdg_timer));

    } else {

        rt_thread_detach(&(prova->thread1));
        rt_thread_detach(&(prova->thread2));

        rt_kprintf("NO BUENO!!! \n");
        rt_kprintf("STATUS_THREAD_1 is %d\n", STATUS_THREAD_1);
        rt_kprintf("STATUS_THREAD_2 is %d\n", STATUS_THREAD_2);

    }

        // Set status flags to UNKNOWN
        STATUS_THREAD_1 = UNKNOWN;
        STATUS_THREAD_2 = UNKNOWN;
}

void wdg_timeout()
{

    rt_kprintf("WATCHDOG is called, time is up!!! \n");

    //// killare e ricreare i thread con init non create


            rt_kprintf("WATCHDOG killed all threads \n");
            rt_thread_delay(RT_TICK_PER_SECOND*5);

            // Ricrea i thread
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

            rt_thread_delay(RT_TICK_PER_SECOND*15);
            rt_kprintf("WATCHDOG recreated all threads \n");
}
