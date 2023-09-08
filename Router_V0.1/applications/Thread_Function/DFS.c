#include "./Thread_Function/DFS.h"


// Function to print a path and its weight
void printPathDfs(int path[], int path_length, int graph[V][V]) {
    int weight = 0;
    extern usr users[MAX_USER];
    extern struct rt_semaphore users_sem;
    char from_s[MAX_CHAR], to_s[MAX_CHAR];

    for (int i = 0; i < path_length - 1; i++) {
        int from = path[i];
        int to = path[i + 1];

        rt_sem_take(&users_sem, RT_WAITING_FOREVER);
        for(int j=0; j<active_user; j++){
            if(from == j)
                rt_strncpy(from_s, users[j].name, sizeof(users[j].name));
            if(to == j)
                rt_strncpy(to_s, users[j].name, sizeof(users[j].name));

        }
        rt_sem_release(&users_sem);

        weight += graph[from][to];
        printf("%s -> %s (Weight: %d) ", from_s, to_s, graph[from][to]);
    }
    printf("\nTotal Weight: %d\n", weight);
}

// Recursive DFS function to find all paths
void findAllPaths(int node, int destination, int graph[V][V],
                  bool visited[V], int path[], int path_length) {
    visited[node] = true;
    path[path_length] = node;
    path_length++;

    if (node == destination) {
        printPathDfs(path, path_length, graph);
    } else {
        for (int i = 0; i < V; i++) {
            if (graph[node][i] != 0 && !visited[i]) {
                findAllPaths(i, destination, graph, visited, path, path_length);
            }
        }
    }

    // Backtrack
    visited[node] = false;
    path_length--;
}

// Function to initialize and call DFS
void dfs(int source, int destination, int graph[V][V]) {
    bool visited[V] = {false};
    int path[V];
    int path_length = 0;

    printf("Paths from %d to %d:\n", source, destination);
    findAllPaths(source, destination, graph, visited, path, path_length);
}

