#include "ultrasonic.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

// Initialize the ultrasonic sensor
void Ultrasonic_Init(void) {
    // Initialize GPIO pins
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

// Trigger the ultrasonic sensor to start a measurement
void Ultrasonic_Trigger(void) {
    // Send a 10us pulse to trigger the sensor
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);
}

// Get the distance measured by the ultrasonic sensor in centimeters
uint32_t Ultrasonic_GetDistance(void) {
    // Trigger the sensor
    Ultrasonic_Trigger();

    printf("Here \n");

    // Wait for the echo to start
    while (gpio_get(ECHO_PIN) == 0);

    // Measure the duration of the echo pulse
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1);
    absolute_time_t end_time = get_absolute_time();

    // Calculate the duration in microseconds
    int64_t duration = absolute_time_diff_us(start_time, end_time);

    // Calculate the distance in centimeters
    uint32_t distance = (duration / 2) / 29.1;

    return distance;
}