#ifndef APPLICATIONS_THREAD_FUNCTION_CONFIG_H_
#define APPLICATIONS_THREAD_FUNCTION_CONFIG_H_

#define MAX_USER 100
#define MAX_CHAR 256
#define MAX_DIST 150

#define true 1
#define false 0

typedef struct user {
    char name[MAX_CHAR];
    int isActive;
    //int position;
    int distance[MAX_USER];
} usr;

usr users[MAX_USER];

int g_matrix[MAX_USER][MAX_USER];

int active_user;

static struct rt_semaphore matrix_sem;

#endif
