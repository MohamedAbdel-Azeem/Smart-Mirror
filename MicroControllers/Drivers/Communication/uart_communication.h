#pragma once
#ifndef UART_COMMUNICATION_H
#define UART_COMMUNICATION_H

#include <stdio.h>
#include <pico/stdlib.h>

// Function to initialize UART
uint uart_init_config(uart_inst_t* uart_id,uint pin_tx, uint pin_rx, uint baud_rate);

// Function to send a message over UART
void uart_send_message( char* message);

// Function to receive a message over UART
char* uart_receive_message();

#endif
