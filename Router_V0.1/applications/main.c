#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "./Thread_Function/Thread_1.h"

int main(void)
{
    int graph[V][V] = { { 0, 1, 4, 0, 0, 0 },
                        { 1, 0, 7, 5, 0, 0 },
                        { 4, 7, 0, 1, 3, 0 },
                        { 0, 5, 1, 0, 2, 6 },
                        { 0, 0, 3, 2, 0, 4 },
                        { 0, 0, 0, 6, 4, 0 } };
    dijkstra(graph, 0);
    return 0;
}

