#include "Thread_Function/wdg.h"
#include <rthw.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void monitor_timeout()
{
    extern rt_flag_t STATUS_THREAD_1;
    extern rt_flag_t STATUS_THREAD_2;
    extern int wdg_status;
    extern rt_thread_t thread1;
    extern rt_thread_t thread2;
    extern struct rt_timer wdg_timer;


    if (STATUS_THREAD_1 == DEAD || STATUS_THREAD_2 == DEAD) {
        wdg_status = DEAD;
        return;
    }

    //rt_tick_t clock_tick;
    //clock_tick = rt_tick_get();
    //rt_kprintf("MONITOR Timer check at %d clock tick\n", clock_tick);

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
            rt_thread_delete(thread2);
            STATUS_THREAD_1 = DEAD;
            STATUS_THREAD_2 = DEAD;
            rt_kprintf("\n");
            rt_kprintf("INFO:   THREAD 1 does not respond! \n");
            rt_kprintf("    STATUS_THREAD_1 is %d\n", STATUS_THREAD_1);
            rt_kprintf("    THREAD 1 killed\n");
            rt_kprintf("\n");
            return;
        }

        if (STATUS_THREAD_2 == UNKNOWN){
            rt_thread_delete(thread1);
            rt_thread_delete(thread2);
            STATUS_THREAD_1 = DEAD;
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
    extern int g_matrix[MAX_USER][MAX_USER];
    extern usr users[MAX_USER];

    if (wdg_status == DEAD) {return;}

    // reset users and g_matrix
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

    active_user = 4;

    // Reset initial users Matrix
    strcpy(users[0].name ,"Francesco_Calice");
    users[0].isActive = TRUE;

    strcpy(users[1].name,"Francesco_Spagnoletti");
    users[1].isActive = TRUE;

    strcpy(users[2].name,"Lucia_Vencato");
    users[2].isActive = TRUE;

    strcpy(users[3].name,"Stefano_Di_Carlo");
    users[3].isActive = TRUE;

    rt_kprintf("WATCHDOG is called, time is up!!! \n");
    rt_kprintf("The killed threads should be recreated!!! \n");
}
