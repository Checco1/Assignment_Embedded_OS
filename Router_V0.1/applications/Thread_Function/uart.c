#include "uart.h"

#define SAMPLE_UART_NAME       "uart1"

/* Semaphore used to receive messages */
struct rt_semaphore rx_sem;
static rt_device_t serial;

/* Receive data callback function */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* After the uart device receives the data, it generates an interrupt, calls this callback function, and then sends the received semaphore. */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;
    char welcome[] = "\nUART Main menu' select one function: \r\n"
                     "u: Retrive user name by position index \r\n"
                     "d: Retrive user minimum distances from all users \r\n"
                     "t: Retrive all possible paths between two users \r\n";

    while (1)
    {
        //Print Welcome Message
        rt_device_write(serial, 0, &welcome, (sizeof(welcome) - 1));
        /* Read a byte of data from the serial port and wait for the receiving semaphore if it is not read */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* Being Suspended and waiting for the semaphore */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* Read the data from the serial port and output through dislocation */
        //Starting from here to develop callbacks function
        //ch = ch + 1;
        rt_device_write(serial, 0, &ch, sizeof(char));

        switch (ch){

        case 'u':
            rt_device_write(serial, 0, "\n", 1);
            char msg[] = "Please insert user's index (ex. 1 = 01):\r\n";
            rt_device_write(serial, 0, &msg, (sizeof(msg) - 1));
            Senduser();
            break;

        case 'd':
            rt_device_write(serial, 0, "\n", 1);
            char msg1[] = "Please insert user's index (ex. 1 = 01):\r\n";
            rt_device_write(serial, 0, &msg1, (sizeof(msg1) - 1));
            bestPathUart();
            break;

        case 't':
            rt_device_write(serial, 0, "\n", 1);
            char msg2[] = "Please insert two user's index (ex. 1 = 01):\r\n";
            rt_device_write(serial, 0, &msg2, (sizeof(msg2) - 1));
            dfsUart();
            break;

        default:
            rt_device_write(serial, 0, "\n", 1);
            char msg3[] = "Error wrong command try again\r\n";
            rt_device_write(serial, 0, &msg3, (sizeof(msg3) - 1));
            break;
        }


    }
}

static int uart_comm(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "Start the UART communication\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* Find uart devices in the system */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* Initialize the semaphore */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* Open the uart device in interrupt receive and polling send mode */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* Set the receive callback function */
    rt_device_set_rx_indicate(serial, uart_input);
    /* Send string */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* Create a serial thread */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 4096, 20, 10);
    /* Start the thread successfully */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}


void Senduser(){

    extern struct rt_semaphore users_sem;
    char sel_1;
    char sel_2;
    char *response;
    int sel1;
    int sel2;

    //Take First Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_1, 1);
    rt_device_write(serial, 0, &sel_1, 1);
    //Take Second Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_2, 1);
    rt_device_write(serial, 0, &sel_2, 1);

    rt_device_write(serial, 0, "\n", 1);

    sel1 = ((int)sel_1 % 48) * 10;
    sel2 = (int)sel_2 % 48;

    sel1 = sel1 + sel2;

    if(sel1 >= 0 && sel1 < 100){
        if (users[sel1].isActive == TRUE){

            rt_sem_take(&users_sem, RT_WAITING_FOREVER);
            //Start Critical Section

            int i = strlen(users[sel1].name);
            response = (char *)malloc( i * sizeof(char));
            strcpy(response,users[sel1].name);

            rt_sem_release(&users_sem);
            //End Critical Section

            strcat(response, "\n");
            rt_device_write(serial, 0, response, ((i+1) * sizeof(char)));
            free(response);
        } else {
            char error[] = "User not active\r\n";
            rt_device_write(serial, 0, error, (sizeof(error) - 1));
        }
    } else {
        char error[] = "Wrong Number\r\n";
        rt_device_write(serial, 0, error, (sizeof(error) - 1));
    }
}

void bestPathUart(){

    extern struct rt_semaphore users_sem;
    char sel_1;
    char sel_2;
    char msg[MAX_CHAR];
    int siz;
    int sel1;
    int sel2;

    //Take First Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_1, 1);
    rt_device_write(serial, 0, &sel_1, 1);
    //Take Second Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_2, 1);
    rt_device_write(serial, 0, &sel_2, 1);

    rt_device_write(serial, 0, "\n", 1);

    sel1 = ((int)sel_1 % 48) * 10;
    sel2 = (int)sel_2 % 48;

    sel1 = sel1 + sel2;

    if(sel1 >= 0 && sel1 < 100){

        rt_sem_take(&users_sem, RT_WAITING_FOREVER);
        //Start Critical Section
        if (users[sel1].isActive == TRUE){

            sprintf(msg, "Best path from %s to all nodes\r\n", users[sel1].name);
            siz = strlen(msg);
            rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));

            for(int j = 0; j<active_user; j++){
                sprintf(msg, "%s to %s: %d\r\n", users[sel1].name, users[j].name, users[sel1].distance[j]);
                siz = strlen(msg);
                rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));
            }

        } else {
            char error[] = "User not active\r\n";
            rt_device_write(serial, 0, error, (sizeof(error) - 1));
        }

        rt_sem_release(&users_sem);
        //End Critical Section
    } else {
        char error[] = "Wrong Number\r\n";
        rt_device_write(serial, 0, error, (sizeof(error) - 1));
    }

}

void dfsUart(){

    extern struct rt_semaphore users_sem;
    char sel_1;
    char sel_2;
    char msg[MAX_CHAR];
    int siz;
    int sel1;
    int sel2;
    int user1;
    int user2;

    //Take First Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_1, 1);
    rt_device_write(serial, 0, &sel_1, 1);
    //Take Second Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_2, 1);
    rt_device_write(serial, 0, &sel_2, 1);

    sel1 = ((int)sel_1 % 48) * 10;
    sel2 = (int)sel_2 % 48;

    user1 = sel1 + sel2;

    rt_device_write(serial, 0, "\n", 1);

    //Take First Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_1, 1);
    rt_device_write(serial, 0, &sel_1, 1);
    //Take Second Number
    rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    rt_device_read(serial, -1, &sel_2, 1);
    rt_device_write(serial, 0, &sel_2, 1);

    sel1 = ((int)sel_1 % 48) * 10;
    sel2 = (int)sel_2 % 48;

    user2 = sel1 + sel2;

    rt_device_write(serial, 0, "\n", 1);

    if( (user1 >= 100 || user1 < 0) || (user2 >= 100 || user2 < 0) ){

        char error[] = "Wrong user's numbers\r\n";
        rt_device_write(serial, 0, error, (sizeof(error) - 1));

    } else {

        rt_sem_take(&users_sem, RT_WAITING_FOREVER);
        //Start Critical Section
        if (users[user1].isActive == TRUE && users[user2].isActive == TRUE){

            sprintf(msg, "All path from %s to %s\r\n", users[user1].name, users[user2].name);
            siz = strlen(msg);
            rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));

            rt_sem_take(&matrix_sem, RT_WAITING_FOREVER);
            dfs_U(user1, user2, g_matrix);
            rt_sem_release(&matrix_sem);

        } else {
            char error[] = "One user is not active\r\n";
            rt_device_write(serial, 0, error, (sizeof(error) - 1));
        }

        rt_sem_release(&users_sem);
        //End Critical Section
    }

}

// Function to print a path and its weight
void sendPathDfsUart(int path[], int path_length, int graph[V][V]) {
    int weight = 0;
    extern usr users[MAX_USER];
    extern struct rt_semaphore users_sem;
    char from_s[MAX_CHAR], to_s[MAX_CHAR];

    char msg[MAX_CHAR];
    int siz;

    for (int i = 0; i < path_length - 1; i++) {
        int from = path[i];
        int to = path[i + 1];

        for(int j=0; j<active_user; j++){
            if(from == j)
                rt_strncpy(from_s, users[j].name, sizeof(users[j].name));
            if(to == j)
                rt_strncpy(to_s, users[j].name, sizeof(users[j].name));

        }

        weight += graph[from][to];

        sprintf(msg, "%s -> %s (Weight: %d) \r\n", from_s, to_s, graph[from][to]);
        siz = strlen(msg);
        rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));

    }

    sprintf(msg, "Total Weight: %d\n\n", weight);
    siz = strlen(msg);
    rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));
}

// Recursive DFS function to find all paths
void findAllPathsUart(int node, int destination, int graph[V][V],
                  bool visited[V], int path[], int path_length) {
    visited[node] = true;
    path[path_length] = node;
    path_length++;

    if (node == destination) {
        sendPathDfsUart(path, path_length, graph);
    } else {
        for (int i = 0; i < V; i++) {
            if (graph[node][i] != 0 && !visited[i]) {
                findAllPathsUart(i, destination, graph, visited, path, path_length);
            }
        }
    }

    // Backtrack
    visited[node] = false;
    path_length--;
}

// Function to initialize and call DFS
void dfs_U(int source, int destination, int graph[V][V]) {
    bool visited[V] = {false};
    int path[V];
    int path_length = 0;

    char msg[MAX_CHAR];
    int siz;

    sprintf(msg, "Paths from %s to %s\r\n\n", users[source].name, users[destination].name);
    siz = strlen(msg);
    rt_device_write(serial, 0, msg, ((siz) * sizeof(char)));

    findAllPathsUart(source, destination, graph, visited, path, path_length);
}

/* Export to the msh command list */
MSH_CMD_EXPORT(uart_comm, uart device communicator);
