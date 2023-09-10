#include "Thread_Function/user.h"


extern usr users[MAX_USER];
extern int g_matrix[MAX_USER][MAX_USER];
extern int active_user;
static char firstUser[MAX_CHAR];
static char secondUser[MAX_CHAR];


//checks that a user exists and is active: returns user flag
int checkUser(char user[MAX_CHAR])
{
    int user_OK=0;  //user flag:  0 -> user does not exist, 1 -> user exists but not active, 2 -> user exists and active
    extern struct rt_semaphore users_sem;

    for(int j = 0; j < strlen(user); j++)
            user[j] = tolower(user[j]);

    rt_sem_take(&users_sem, RT_WAITING_FOREVER);

    //check that user exists and is active
    for(int i=0; i<MAX_USER; i++){
        for(int j = 0; j<strlen(users[i].name); j++)
            users[i].name[j] = tolower(users[i].name[j]);

        if(rt_strcmp(users[i].name, user) == 0){
            user_OK = 1;
            if(users[i].isActive == 1){
                user_OK = 2;
            }
        }
    }

    rt_sem_release(&users_sem);

    if(user_OK == 0)
           rt_kprintf("Selected user does not exist. Try again\n");
    if(user_OK == 1)
           rt_kprintf("Selected user exists but not active. Try again\n");
    if(user_OK == 2)
           rt_kprintf("Selected user exists and is active: %s\n", user);

    return user_OK;

}

//print the number of active users
void activeUsers(){
   rt_kprintf("ACTIVE USERS = %d\n", active_user);
}
//search for not active user, for creating a new active one
void createUser(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;
    char user[MAX_CHAR];
    int newUser = 0; //flag

    if (argc > 1)
        rt_strncpy(user, argv[1], sizeof(user));

    rt_sem_take(&users_sem, RT_WAITING_FOREVER);
    for(int i=0; i < MAX_USER; i++){
        if(users[i].isActive == FALSE){
            rt_strncpy(users[i].name, user, sizeof(user));
            users[i].isActive = TRUE;
            newUser=1;
            break;
        }
    }
    rt_sem_release(&users_sem);

    if(newUser == 1){
        rt_kprintf("Now user %s has been created\n", user);
        active_user++;
    }
    else {
        rt_kprintf("Impossible to create new user: network is full\n");
    }

}

//receives a user, check if it exists and if the user is active, it set it NOT ACTIVE.
void deactivateUser(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;
    char user[MAX_CHAR];

    //save command input in variables
    if (argc > 1)
        rt_strncpy(user, argv[1], sizeof(user));

    if(checkUser(user) == 2){
        rt_sem_take(&users_sem, RT_WAITING_FOREVER);

        for(int i=0; i < MAX_USER; i++){
            if(rt_strcmp(users[i].name, user) == 0){
                users[i].isActive = FALSE;
                active_user--;
                rt_kprintf("Now user %s is not active\nNumber of active user: %d\n", user, active_user);
            }
        }

        rt_sem_release(&users_sem);
    }
}

//modify the name of a user: receives as first input the user to modify and as second one the new name of the user
void modifyUserName(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;
    char userName[MAX_CHAR], userNewName[MAX_CHAR];
    if (argc > 1){

           rt_strncpy(userName, argv[1], sizeof(userName));
           rt_strncpy(userNewName, argv[2], sizeof(userNewName));
    }

    if(checkUser(userName) == 2){

        rt_sem_take(&users_sem, RT_WAITING_FOREVER);
        for(int i=0; i < MAX_USER; i++){
           if(rt_strcmp(users[i].name, userName) == 0){
               rt_strncpy(users[i].name, userNewName, sizeof(userNewName));
               rt_kprintf("%s has been changed to %s\n", userName, users[i].name);
           }
        }
        rt_sem_release(&users_sem);
    } else {
        rt_kprintf("Impossible to modify user name %s because it doesn't exist or is inactive\n", userName);
    }
}

//it shows the minimum path between two users
void showAllPaths(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;
    extern struct rt_semaphore matrix_sem;
    int src, dst;
    if (argc > 1){
            rt_strncpy(firstUser, argv[1], sizeof(firstUser));
            rt_strncpy(secondUser, argv[2], sizeof(secondUser));
    }

    if((checkUser(firstUser) == 2) && (checkUser(secondUser) == 2)){

        rt_sem_take(&users_sem, RT_WAITING_FOREVER);

        for(int i=0; i < MAX_USER; i++){
            if(rt_strcmp(users[i].name, firstUser) == 0)
                src = i;
            if(rt_strcmp(users[i].name, secondUser) == 0)
                dst = i;
        }
        rt_sem_release(&users_sem);

        rt_kprintf("src %d, dst %d \n", src, dst);
        rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);
        dfs(src, dst, g_matrix);
        rt_sem_release(&matrix_sem);


    } else {
        rt_kprintf("Error selected user not valid\n");
    }

}
//print the length of the best path from a user to all the others
void bestPathAll(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;

    //save command input in variables
    if (argc > 1)
        rt_strncpy(firstUser, argv[1], sizeof(firstUser));

    if(checkUser(firstUser) == 2){
        rt_sem_take(&users_sem, RT_WAITING_FOREVER);

        for(int i=0; i < MAX_USER; i++){
            if(rt_strcmp(users[i].name, firstUser) == 0){
                rt_kprintf("Best path from %s to all nodes\n", firstUser);
                for(int j = 0; j<active_user; j++)
                    rt_kprintf("%s to %s: %d\n", firstUser, users[j].name, users[i].distance[j]);
            }
        }

        rt_sem_release(&users_sem);
    } else {
        rt_kprintf("Error selected user not valid\n");
    }

}

//print the length of the best path from two selected users
void bestPathTwoNodes(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;

    if (argc > 1){
        rt_strncpy(firstUser, argv[1], sizeof(firstUser));
        rt_strncpy(secondUser, argv[2], sizeof(secondUser));
    }

    if((checkUser(firstUser) == 2) && (checkUser(secondUser) == 2)){
        rt_sem_take(&users_sem, RT_WAITING_FOREVER);

        for(int i=0; i < MAX_USER; i++){
            if(rt_strcmp(users[i].name, firstUser) == 0){
                for(int j=0; j<MAX_USER; j++){
                    if(rt_strcmp(users[j].name, secondUser) == 0){
                        rt_kprintf("Best path from %s to %s: %d\n", firstUser, secondUser, users[i].distance[j]);
                    }
                }
            }
        }

        rt_sem_release(&users_sem);
    } else {
        rt_kprintf("Error selected users not valid\n");
    }

}

MSH_CMD_EXPORT(activeUsers, print the number of active users in the network);
MSH_CMD_EXPORT(createUser, used to create a new active user if there are some inactive ones: receives one input);
MSH_CMD_EXPORT(deactivateUser, used to modify a user state from ACTIVE to NOT ACTIVE: receives one input);
MSH_CMD_EXPORT(modifyUserName, used to modify user name: receives two inputs);
MSH_CMD_EXPORT(showAllPaths, used to show all possible path between two users: receives two inputs);
MSH_CMD_EXPORT(bestPathAll, used to know the best path from a user to all the others: receives one input);
MSH_CMD_EXPORT(bestPathTwoNodes, used to know the best path from two selected users: receives two inputs);
