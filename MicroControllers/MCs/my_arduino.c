#include "my_arduino.h"
#include "pico/stdlib.h"
#include "Drivers/Actuators/buzzer.h"
#include "Drivers/Actuators/led_strip.h"
#include "Drivers/Sensors/ultrasonic.h"
#include "Drivers/Communication/uart_communication.h"

#define button_pin 27


void handle_close_buzzer(uint gpio, uint32_t events){
    printf("Button pressed, buzzer should be off \n");
    Buzzer_Off();
}


void button_init(){
    //button in pin 14
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);

    // Set up GPIO interrupt for button press (rising edge only)
    gpio_set_irq_enabled_with_callback(button_pin, GPIO_IRQ_EDGE_RISE, true, &handle_close_buzzer);
    // Set priority for the GPIO interrupt (higher priority if needed)
    irq_set_priority(IO_IRQ_BANK0, 0); // Set GPIO interrupt to highest priority
    // Enable the GPIO interrupt using irq_set_enabled  
    irq_set_enabled(IO_IRQ_BANK0, true); // Enable GPIO interrupt
}

int my_arduino_main(){
    // button_init();
    Buzzer_Init();
    Buzzer_On();
    LED_Strip_Init();


    while (true)
    {
        LED_Strip_Turn_ON();
        sleep_ms(2000);
        LED_Strip_Turn_OFF();
    }
    
}