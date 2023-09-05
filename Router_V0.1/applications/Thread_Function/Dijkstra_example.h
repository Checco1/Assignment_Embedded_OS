#ifndef DIJKSTRA_EXAMPLE_H
#define DIJKSTRA_EXAMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <rtthread.h>
#include "./Thread_Function/config.h"
#define V 100

void printSolution(int dist[], int parent[], int src);
int findMinDistance(int dist[], int visited[]);
void printPath(int parent[], int j);
void dijkstra(int graph[V][V], int src);

void updatestruct(int dist[], int src);


#endif
