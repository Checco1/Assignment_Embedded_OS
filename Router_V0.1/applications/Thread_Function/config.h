#ifndef APPLICATIONS_THREAD_FUNCTION_CONFIG_H_
#define APPLICATIONS_THREAD_FUNCTION_CONFIG_H_

#define MAX_USER 100
#define MAX_CHAR 256
#define MAX_DIST 150

#define true 1
#define false 0
#define ALIVE 0
#define UNKNOWN 1
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
static struct rt_thread thread1;
static char thread1_stack[1024];
static struct rt_thread thread2;
static char thread2_stack[1024];
static struct rt_thread watchdog;

//Pointers for Semaphore
static struct rt_semaphore active_sem;
static struct rt_semaphore matrix_sem;
static struct rt_semaphore users_sem;

//Pointers for timers
static struct rt_timer wdg_monitor_timer;
static struct rt_timer wdg_timer;

//Thread status flags
static rt_flag_t STATUS_THREAD_1;
static rt_flag_t STATUS_THREAD_2;

#endif
