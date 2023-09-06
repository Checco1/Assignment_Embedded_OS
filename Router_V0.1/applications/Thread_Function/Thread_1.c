#include "Thread_Function/Thread_1.h"
#include "Thread_Function/Thread_2.h"

void random_mixer(){

    extern struct rt_semaphore matrix_sem;
    extern int g_matrix[MAX_USER][MAX_USER];
    extern usr users[MAX_USER];
    extern int active_user;

    int randomUsr1;
    int randomUsr2;
    int randomDist;

    while(1){
        if (active_user != 0){

            rt_sem_take(&users_sem, RT_WAITING_FOREVER);
            //Starting Critical Section USER STRUCT
            do {
                randomUsr1 = rand() % active_user;
            } while (users[randomUsr1].isActive != TRUE);

            do {
                randomUsr2 = rand() % active_user;
                if (randomUsr2 != randomUsr1) {
                    if (users[randomUsr2].isActive == TRUE){
                        break;
                    }
                }
            } while (1);
            //Ending Critical Section USER STRUCT
            rt_sem_release(&users_sem);

            do {
                randomDist = rand() % MAX_DIST;
            } while (randomDist == 0);

            //printf("Random User 1: %d\n", randomUsr1);
            //printf("Random User 2: %d\n", randomUsr2);
            //printf("Random Distance: %d\n", randomDist);

            rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);

            g_matrix[randomUsr1][randomUsr2] = randomDist;
            g_matrix[randomUsr2][randomUsr1] = randomDist;

            rt_sem_release(&matrix_sem);

            rt_kprintf("Matrix Updated!\n");

            STATUS_THREAD_1 = ALIVE;
            rt_thread_delay(RT_TICK_PER_SECOND*20);
        }
    }
};

void backup(){

    extern rt_flag_t STATUS_THREAD_1;
    extern rt_flag_t STATUS_THREAD_2;
    extern rt_thread_t thread1;
    extern rt_thread_t thread2;
    extern struct rt_semaphore matrix_sem;

    rt_err_t ret;

    if ((STATUS_THREAD_1 != DEAD) && (STATUS_THREAD_2 != DEAD)) {
        rt_kprintf("INFO:   No THREAD is dead \n");
        rt_kprintf("\n");

    }

    if (STATUS_THREAD_1 == DEAD) {
        thread1 = rt_thread_create("thread1",
                                random_mixer, RT_NULL,
                                1024,
                                30, 5);

        ret = rt_thread_startup(thread1);
        if (ret != RT_EOK) {
            rt_kprintf("ERROR:   THREAD 1 not recreated! \n");
            rt_kprintf("\n");
        } else {
            rt_kprintf("INFO:   THREAD 1 recreated! \n");
            rt_kprintf("\n");
        }
    }

    if (STATUS_THREAD_2 == DEAD) {
        thread2 = rt_thread_create("thread2",
                                solver, RT_NULL,
                                4096,
                                20, 5);

        ret = rt_thread_startup(thread2);
        if (ret != RT_EOK) {
            rt_kprintf("ERROR:   THREAD 2 not recreated! \n");
        } else {
            rt_kprintf("INFO:   THREAD 2 recreated! \n");
        }
    }
};

void printer(){

    for(int i = 0; i < active_user; i++){
        for (int j = 0; j < active_user; j++){
            printf("%d ", g_matrix[i][j]);
        }
        printf("\n");
    }
}

//MSH_CMD_EXPORT(random_mixer, Random_Generator);
MSH_CMD_EXPORT(backup, Create thread_1_&_2);
MSH_CMD_EXPORT(printer, Print_Matrix);
