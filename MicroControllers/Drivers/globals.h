#ifndef GLOBALS_H
#define GLOBALS_H

#include "stdint.h"
#include "cJSON.h"

#define MAX_ITEMS 100

extern volatile uint64_t current_time;
extern volatile uint64_t buzzer_time;
extern cJSON *news_array[MAX_ITEMS];
extern int news_count;


#endif