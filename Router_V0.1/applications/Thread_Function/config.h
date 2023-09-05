#ifndef APPLICATIONS_THREAD_FUNCTION_CONFIG_H_
#define APPLICATIONS_THREAD_FUNCTION_CONFIG_H_

#define MAX_USER 100
#define MAX_CHAR 256
#define MAX_DIST 150

#define true 1
#define false 0
#define ALIVE 0
#define UNKNOWN 1
#define DEAD 2
#define WDG_MONITOR_TIME_SEC 12
#define WDG_TIME_SEC 15

//Custom structure used by the modem
typedef struct user {
    char name[MAX_CHAR];
    int isActive;
    //int position;
    int distance[MAX_USER];
} usr;

usr users[MAX_USER];
int g_matrix[MAX_USER][MAX_USER];
int active_user;

//Pointers for threads
rt_thread_t thread1;
rt_thread_t thread2;
struct rt_thread watchdog;

//Pointers for Semaphore
struct rt_semaphore active_sem;
struct rt_semaphore matrix_sem;
struct rt_semaphore users_sem;

//Pointers for timers
struct rt_timer wdg_monitor_timer;
struct rt_timer wdg_timer;

//Thread status flags
rt_flag_t STATUS_THREAD_1;
rt_flag_t STATUS_THREAD_2;

#endif
