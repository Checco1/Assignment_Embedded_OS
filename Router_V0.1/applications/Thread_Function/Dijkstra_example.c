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

// Funzione per stampare le distanze e i percorsi più brevi
void printSolution(int dist[], int parent[], int src) {
    printf("Nodo \t Distanza \t Percorso\n");
    for (int i = 0; i < active_user; i++) {
        printf("%d \t %d \t\t ", i, dist[i]);
        printPath(parent, i);
        printf("\n");
    }
};

// Funzione per trovare l'indice del nodo con la distanza minima
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

// Funzione per stampare il percorso più breve
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d ", j);
        return;
    }
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// Funzione per l'algoritmo di Dijkstra
void dijkstra(int graph[V][V], int src) {
    int dist[V]; // Array delle distanze minime
    int visited[V]; // Array dei nodi visitati
    int parent[V]; // Array dei nodi predecessori
    // Inizializzazione
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;
    // Ciclo principale
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
    // Stampa la soluzione
    printSolution(dist, parent, src);
}

void example(){
    int graph[V][V] = { { 0, 1, 4, 0, 0, 0 },
                        { 1, 0, 7, 5, 0, 0 },
                        { 4, 7, 0, 1, 3, 0 },
                        { 0, 5, 1, 0, 2, 6 },
                        { 0, 0, 3, 2, 0, 4 },
                        { 0, 0, 0, 6, 4, 0 } };
    dijkstra(graph, 0);
}

/* export the msh command */
MSH_CMD_EXPORT(example, dij);
