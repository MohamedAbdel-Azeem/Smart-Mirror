#include "pico.h"

#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include <string.h>
#include "pico/stdlib.h"
#include "time.h"
#include "pico/time.h"
#include "Drivers/globals.h"
#include "Drivers/Communication/myWifi.h"
#include "hardware/irq.h"
#include "Drivers/Actuators/buzzer.h"
#include "Drivers/Actuators/TFT_arduino.h"
#include "Drivers/Sensors/adc_temperature.h"
#include "Drivers/Actuators/led_strip.h"

// Global variables
volatile uint64_t current_time = 0; // Seconds since epoch
volatile uint64_t buzzer_time = 0; // Time to trigger the buzzer
cJSON *news_array[MAX_ITEMS];
int news_count = 0;
int currNewsIndex = 0;

bool time_start = false;

float currTemp = 0.0;


struct repeating_timer repeating_timer;
struct repeating_timer temperature_timer;
struct repeating_timer news_timer;


// // Function to convert and print the current timestamp
char* get_string_time(uint64_t current_time) {
    // Convert milliseconds to seconds
    time_t timestamp_seconds = current_time / 1000;


    // Convert to struct tm (UTC time)
    struct tm *local_time = gmtime(&timestamp_seconds);
    local_time->tm_hour += 2; // Adjust for GMT+2
    mktime(local_time); // Normalize the time structure

    // Print formatted time
    char *time_string = malloc(20); // Allocate memory for the time string
    sprintf(time_string, "%04d-%02d-%02d %02d:%02d:%02d",
        local_time->tm_year + 1900,
        local_time->tm_mon + 1,
        local_time->tm_mday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec);
    return time_string;
}

// // Function to update timestamp
void update_timestamp() {
    if (current_time == 0) {
        return;
    } else {
        if (! time_start) {
            Lcd_Clear(0x0000,2); // Clear the screen
            time_start = true;
            char* time_string = get_string_time(current_time);
            LCD_Draw_String(20, 20, time_string, 0xFFFF, 2.0 , 2);
        } else {
            current_time += 1000; // Increment by 1 second
            char* old_time_string = get_string_time(current_time - 1000);
            char* time_string = get_string_time(current_time);
            // LCD_Draw_String(20, 150, time_string, 0xFFFF, 2.0 , 2);
            UpdateDisplay(old_time_string, time_string, 20, 20, 0xFFFF, 0x0000, 2.0, 2);
        }
    }
}



bool update_time_callback(struct repeating_timer *t) {
    update_timestamp();  // Call your existing function to update the timestamp
    return true;  // Return true to keep the timer repeating
}

// // Function to initialize a periodic timer that fires every 1 second
void start_periodic_timer() {
    // Set a periodic timer to fire every 1 second (1000 ms)
    add_repeating_timer_ms(1000, update_time_callback, NULL, &repeating_timer);
}

bool update_temperature_callback(struct repeating_timer *t) {
    char* oldTemp = malloc(10);
    sprintf(oldTemp, "%.2f C", currTemp);
    currTemp = read_temperature();
    char* temp_string = malloc(10);
    sprintf(temp_string, "%.2f C", currTemp);
    UpdateDisplay(oldTemp, temp_string, 100, 200, 0xFFFF, 0x0000, 2.0, 2);
    free(temp_string);
    free(oldTemp);
    return true;
}

void start_temperature_timer() {
    // Set a periodic timer to fire every 1 second (1000 ms)
    add_repeating_timer_ms(1000 * 20, update_temperature_callback, NULL, &temperature_timer);
}

bool update_news_callback(struct repeating_timer *t) {
    Lcd_Clear(0x0000,1); // Clear the screen
    if (news_count == 0) {
        LCD_Draw_String(20, 20, "No news available", 0xFFFF, 2.0 , 1);
        return true;
    }
    cJSON *currNews = news_array[currNewsIndex];
    char* author = cJSON_GetObjectItem(currNews, "author")->valuestring;
    char* title = cJSON_GetObjectItem(currNews, "title")->valuestring;
    char* description = cJSON_GetObjectItem(currNews, "description")->valuestring;

    LCD_Draw_String(20, 10, title, 0xFFFF, 2.0 , 2);
    char* author_string = malloc(strlen(author) + 4);
    sprintf(author_string, "- %s -", author);
    LCD_Draw_String(135, 60, author_string, 0xFFFF, 2.0 , 1);
    free(author_string);
    LCD_Draw_String(160, 10, description, 0xFFFF, 2.0 , 1);

    currNewsIndex = (currNewsIndex + 1) % news_count;
    return true;  // Return true to keep the timer repeating
}


void start_news_timer() {
    // Set a periodic timer to fire every 1 second (1000 ms)
    add_repeating_timer_ms(1000 * 20, update_news_callback, NULL, &news_timer);
}

void handle_buzzer() {
    if (buzzer_time == 0) {
        Buzzer_Off();
        return;
    }
    if (buzzer_time <= current_time) {
        Buzzer_On();
        return;
    }
}



int my_pico_main() {
    stdio_init_all();
    printf("Starting Pico W HTTP server...\n");

    temp_sensor_init();
    currTemp = read_temperature();

    TFT_init(1);
    TFT_init(2);

    Lcd_Clear(0x0000,1); // Clear the screen
    LCD_Draw_String(120, 100, "Connecting to Wifi ...", 0xFFFF, 2.0 , 1);
    Lcd_Clear(0x0000,2); // Clear the screen
    LCD_Draw_String(20, 20, "Greet the Pico from the Server", 0xFFFF, 2.0 , 2);

    LCD_Draw_String(100, 20, "Temperature: ", 0xFFFF, 2.0 , 2);
    char* temp_string = malloc(10);
    sprintf(temp_string, "%.2f C", currTemp);
    LCD_Draw_String(100, 200, temp_string, 0xFFFF, 2.0 , 2);
    free(temp_string);

    // Connect to Wi-Fi
    char* ipAddress = connect_to_wifi();

    // Initialize the HTTP server
    http_server_init();

    Lcd_Clear(0x0000,1); // Clear the screen
    char* connectMessage = malloc(strlen("Connected to Wifi, Server IP Address is ") + strlen(ipAddress) + 1);
    sprintf(connectMessage, "Connected to Wifi, Server IP Address is %s", ipAddress);;
    LCD_Draw_String(20, 20, connectMessage, 0xFFFF, 2.0 , 1);
    

    // Start the periodic timer
    start_periodic_timer();
    start_temperature_timer();
    start_news_timer();

    LED_Strip_Init();

    // Main loop
    while (true) {
        // handle_buzzer();
        tight_loop_contents();
    }

    return 0;
}
