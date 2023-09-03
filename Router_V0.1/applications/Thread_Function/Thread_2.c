#include "Thread_Function/Thread_2.h"

extern usr users[MAX_USER];
extern int g_matrix[MAX_USER][MAX_USER];
extern int active_user;

void solver(){

    for(int i = 0; i < active_user; i++){
        dijkstra(g_matrix, i);
    }

}


MSH_CMD_EXPORT(solver, Solve_Matrix);
