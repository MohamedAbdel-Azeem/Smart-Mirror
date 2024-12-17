#include "adc_temperature.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"


void temp_sensor_init(){
    adc_init();

    // Select ADC input 4 (Temperature sensor)
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
}


float read_temperature(){
    // Read the raw ADC value
    uint16_t raw = adc_read();

    // Convert ADC value to voltage (3.3V reference and 12-bit ADC)
    const float conversion_factor = 3.3f / (1 << 12); // (1 << 12) = 4096
    float voltage = raw * conversion_factor;

    // Convert voltage to temperature (RP2040 datasheet formula)
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

    return temperature;
}