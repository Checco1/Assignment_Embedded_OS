#ifndef UART
#define UART

#include <rtthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./Thread_Function/config.h"

static rt_err_t uart_input(rt_device_t dev, rt_size_t size);
static void serial_thread_entry(void *parameter);
static int uart_comm(int argc, char *argv[]);
void Senduser();
void bestPathUart();

#endif
