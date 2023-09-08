#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#include "./Thread_Function/DFS.h"

void bestPathTwoNodes(int argc, char *argv[]);
void bestPathAll(int argc, char *argv[]);
int checkUser(char user[MAX_CHAR]);
void deactivateUser(int argc, char *argv[]);
void createUser(int argc, char *argv[]);
void modifyUserName(int argc, char *argv[]);
void activeUsers();

#endif
