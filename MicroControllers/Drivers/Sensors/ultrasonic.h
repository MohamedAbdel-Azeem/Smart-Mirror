#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "stdint.h"

// GPIO pins for the HC-SR04 sensor
#define TRIG_PIN 16
#define ECHO_PIN 17

// Initializes the ultrasonic sensor
void Ultrasonic_Init(void);

// Triggers the ultrasonic sensor to start a measurement
void Ultrasonic_Trigger(void);

// Gets the distance measured by the ultrasonic sensor in centimeters
uint32_t Ultrasonic_GetDistance(void);

#endif // ULTRASONIC_H