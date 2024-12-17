#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "led_strip.h"

void LED_Strip_Init() {
    gpio_init(LED_Strip_Pin);
    gpio_set_dir(LED_Strip_Pin,GPIO_OUT);
    gpio_put(LED_Strip_Pin,1); // Turn off the LED strip, Negative Logic
}


void LED_Strip_Turn_ON() {
    // negative logic so 0 = ON
    gpio_put(LED_Strip_Pin,0);
}   

void LED_Strip_Turn_OFF() {
    //negative logic so 1 = OFF
    gpio_put(LED_Strip_Pin,1);
}
