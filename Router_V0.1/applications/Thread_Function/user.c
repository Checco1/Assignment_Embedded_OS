#include "Thread_Function/user.h"


extern usr users[MAX_USER];
extern int g_matrix[MAX_USER][MAX_USER];
extern int active_user;
static char firstUser[MAX_CHAR];
static char secondUser[MAX_CHAR];

/*void createUser(){

}

void modifiedUser(){

}

void deleteUser(){

}

void allPossiblePath(){

}
*/

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

//print the length of the best path from a user to all the others
void bestPathAll(int argc, char *argv[]){

    extern struct rt_semaphore users_sem;

    //save command input in variables
    if (argc > 1)
        rt_strncpy(firstUser, argv[1], sizeof(firstUser));

    if(checkUser(firstUser) == 2){
        rt_sem_take(&users_sem, RT_WAITING_FOREVER);

        for(int i=0; i < active_user; i++){
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

        for(int i=0; i < active_user; i++){
            if(rt_strcmp(users[i].name, firstUser) == 0){
                for(int j=0; j<active_user; j++){
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

MSH_CMD_EXPORT(bestPathAll, used to know the best path from a user to all the others: receives one input);
MSH_CMD_EXPORT(bestPathTwoNodes, used to know the best path from two selected users: receives two inputs);
