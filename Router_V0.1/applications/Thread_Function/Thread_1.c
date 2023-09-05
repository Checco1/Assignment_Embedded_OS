#include "Thread_Function/Thread_1.h"
#include "Thread_Function/Thread_2.h"

void random_mixer(){

    int randomUsr1;
    int randomUsr2;
    int randomDist;

    extern struct rt_semaphore matrix_sem;
    extern int g_matrix[MAX_USER][MAX_USER];

    while(1){
        if (active_user != 0){
            randomUsr1 = rand() % active_user;
            do {
                randomUsr2 = rand() % active_user;
            } while (randomUsr1 == randomUsr2);

            randomDist = rand() % MAX_DIST;

            //printf("Random User 1: %d\n", randomUsr1);
            //printf("Random User 2: %d\n", randomUsr2);
            //printf("Random Distance: %d\n", randomDist);

            rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);

            g_matrix[randomUsr1][randomUsr2] = randomDist;
            g_matrix[randomUsr2][randomUsr1] = randomDist;

            rt_sem_release(&matrix_sem);

            rt_kprintf("Matrix Updated!\n");

            STATUS_THREAD_1 = ALIVE;
            rt_thread_delay(RT_TICK_PER_SECOND*5);
        }
    }
};

void backup(){

    extern rt_thread_t thread1;
    extern rt_thread_t thread2;
    extern struct rt_semaphore matrix_sem;

    thread1 = rt_thread_create("thread1",
                            random_mixer, RT_NULL,
                            1024,
                            30, 5);

    rt_thread_startup(thread1);

    thread2 = rt_thread_create("thread2",
                            solver, RT_NULL,
                            4096,
                            20, 5);

    rt_thread_startup(thread2);
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
MSH_CMD_EXPORT(backup, Create thread_1);
MSH_CMD_EXPORT(printer, Print_Matrix);
