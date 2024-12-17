#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "buzzer.h"
#include "hardware/clocks.h"
#include "pico/time.h"
#include "stdio.h"

struct repeating_timer buzzer_repeating_timer;


// Global variable to keep track of the current volume state
volatile bool full_volume = false;

// Global variable to keep track of the buzzer state
volatile bool buzzer_on = false;

// Timer callback function to switch volume
bool switch_volume_callback(struct repeating_timer *t) {
    if (buzzer_on) {
        full_volume = !full_volume; // Toggle volume state
        uint slice_num = pwm_gpio_to_slice_num(Buzzer_pin);
        if (full_volume) {
            pwm_set_gpio_level(Buzzer_pin, 65535); // Full volume
        } else {
            pwm_set_gpio_level(Buzzer_pin, 65535 / 2); // No volume
        }
        return true; // Keep repeating
    } else {
        // Ensure the PWM is disabled when the buzzer is off
        uint slice_num = pwm_gpio_to_slice_num(Buzzer_pin);
        pwm_set_gpio_level(Buzzer_pin, 0);
        return false; // stop repeating
    } 
}

void Buzzer_Init() {
    gpio_set_function(Buzzer_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Buzzer_pin);
    pwm_set_enabled(slice_num, true);
    // add_repeating_timer_ms(2000, switch_volume_callback, NULL, &buzzer_repeating_timer);
}

void Buzzer_On() {
    buzzer_on = true;
    // Start the repeating timer
    add_repeating_timer_ms(2000, switch_volume_callback, NULL, &buzzer_repeating_timer);
}

void Buzzer_Off() {
    buzzer_on = false;
    // Stop the repeating timer
    cancel_repeating_timer(&buzzer_repeating_timer);
    uint slice_num = pwm_gpio_to_slice_num(Buzzer_pin);
    pwm_set_gpio_level(Buzzer_pin, 0);
}