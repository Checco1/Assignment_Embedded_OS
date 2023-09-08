#ifndef DFS_H
#define DFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>
#include <stdbool.h>
#include "./Thread_Function/config.h"
#define V 100

void dfs(int source, int destination, int graph[V][V]);
void findAllPaths(int node, int destination, int graph[V][V], bool visited[V], int path[], int path_length);
void printPathDfs(int path[], int path_length, int graph[V][V]) ;

#endif
