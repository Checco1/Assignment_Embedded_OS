#include "Thread_Function/Thread_1.h"

void random_mixer(struct rt_semaphore *matrix_sem){

    int randomUsr1;
    int randomUsr2;
    int randomDist;
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

            rt_sem_take(matrix_sem, RT_WAITING_FOREVER);

            g_matrix[randomUsr1][randomUsr2] = randomDist;
            g_matrix[randomUsr2][randomUsr1] = randomDist;

            rt_sem_release(matrix_sem);

            rt_kprintf("Matrix Updated!\n");

            rt_thread_delay(500);
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
MSH_CMD_EXPORT(printer, Print_Matrix);
