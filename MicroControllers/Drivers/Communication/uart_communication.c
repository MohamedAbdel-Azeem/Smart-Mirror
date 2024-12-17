#include "uart_communication.h"


// Values for UART communication
static  uart_inst_t* UART_ID;
static  uint TX_PIN;
static  uint RX_PIN;
static  uint UART_BAUD_RATE;


// Initialize UART
uint uart_init_config(uart_inst_t* uart_id, uint pin_tx, uint pin_rx, uint baud_rate) {
    // set the UART parameters
    UART_ID = uart_id;
    TX_PIN = pin_tx;
    RX_PIN = pin_rx;
    UART_BAUD_RATE = baud_rate;

    UART_BAUD_RATE = uart_init(UART_ID, UART_BAUD_RATE);  // Initialize UART

    

    gpio_set_function(TX_PIN, GPIO_FUNC_UART);  // Set TX pin for UART
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);  // Set RX pin for UART
    // Enable UART FIFO
    uart_set_fifo_enabled(UART_ID, true);

    gpio_pull_up(RX_PIN);    // Enable pull-up on RX pin

    return UART_BAUD_RATE;  // Return the baud rate
    }

// Send a message over UART
void uart_send_message(char* message) {
    // Send the message character by character
    while (*message) {
        uart_putc(UART_ID, *message);
        message++;
        }
    
    }

// Receive a message over UART
char* uart_receive_message() {
    static char received_msg[256];  // Static buffer for storing received message
    int index = 0;

    // Wait until data is available in the RX FIFO
    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);  // Read a character from UART
        if (c == '\n' || c == '\r') { // End of message (newline or carriage return)
            received_msg[index] = '\0';  // Null-terminate the string
            return received_msg;         // Return the received message
        } else {
            received_msg[index++] = c;  // Store the character in the buffer
            if (index >= sizeof(received_msg) - 1) {
                received_msg[index] = '\0';  // Null-terminate if buffer is full
                return received_msg;
            }
        }
    }

    return NULL;  // Return NULL if no message received
}