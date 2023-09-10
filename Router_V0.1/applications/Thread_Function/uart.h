#ifndef UART
#define UART

#include <rtthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./Thread_Function/config.h"
#define V 100


static rt_err_t uart_input(rt_device_t dev, rt_size_t size);
static void serial_thread_entry(void *parameter);
static int uart_comm(int argc, char *argv[]);
void Senduser();
void bestPathUart();
void dfsUart();
void sendPathDfsUart(int path[], int path_length, int graph[V][V]);
void findAllPathsUart(int node, int destination, int graph[V][V],bool visited[V], int path[], int path_length);
void dfs_U(int source, int destination, int graph[V][V]);

#endif
