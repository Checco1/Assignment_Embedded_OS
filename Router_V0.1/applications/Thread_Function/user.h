#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"

void bestPathTwoNodes(int argc, char *argv[]);
void bestPathAll(int argc, char *argv[]);
int checkUser(char user[MAX_CHAR]);

#endif
