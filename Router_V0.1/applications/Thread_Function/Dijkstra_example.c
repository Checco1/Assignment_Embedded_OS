/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-05     Francesco       the first version
 */
#include <Thread_Function/Dijkstra_example.h>

extern int active_user;

// Function to print the shortest paths
void printSolution(int dist[], int parent[], int src) {
    printf("Node \t Distance \t Path\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t %d \t\t ", i, dist[i]);
        printPath(parent, i);
        printf("\n");
    }
};

//Function to find the index of minimum distance node
int findMinDistance(int dist[], int visited[]) {
    int min = INT_MAX, minIndex;
    for (int i = 0; i < V; i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Function to print the shortest path
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d ", j);
        return;
    }
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// Function for Dijkstra Algorithm
void dijkstra(int graph[V][V], int src) {
    int dist[V]; // Array of minimum distances
    int visited[V]; // Array of visited nodes
    int parent[V]; // Array of predecessors
    // Initialization
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;
    // Main Cycle
    for (int count = 0; count < V-1; count++) {
        int u = findMinDistance(dist, visited);
        visited[u] = 1;
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    updatestruct(dist, src);

    // Print the solution
    //printSolution(dist, parent, src);
}

void updatestruct(int dist[], int src){

    extern usr users[MAX_USER];
    extern struct rt_semaphore users_sem;

    rt_sem_take(&users_sem, RT_WAITING_FOREVER);

    for(int i=0; i<MAX_USER; i++){
        if(dist[i] != INT_MAX){
            //Users connected
            users[src].distance[i] = dist[i];
        }
        else {
            //Users not connected
            users[src].distance[i] = -1;
        }
    }

    rt_sem_release(&users_sem);

};
