#ifndef DIJKSTRA_EXAMPLE_H
#define DIJKSTRA_EXAMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <rtthread.h>
#define V 6

void printSolution(int dist[], int parent[], int src);
int findMinDistance(int dist[], int visited[]);
void printPath(int parent[], int j);
void dijkstra(int graph[V][V], int src);
void example();


#endif
