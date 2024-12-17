#include <stdio.h>
#include "string.h"
#include "cJSON.h"
#include "stdlib.h"
#include "commandExec.h"
#include "Drivers/Actuators/led_strip.h"
#include "globals.h"


void greeting_executer(cJSON *json) {
    cJSON *timestamp = cJSON_GetObjectItem(json, "timestamp");
    if (cJSON_IsString(timestamp) && (timestamp->valuestring != NULL)) {
        current_time = strtoll(timestamp->valuestring, NULL, 10);
    } else {
        printf("Time not found or not a string\n");
    }
    return;
}


void data_executer(cJSON *json) {
    printf("Data Executer\n");

    // Extract the fields from the JSON object
    cJSON *author = cJSON_GetObjectItem(json, "author");
    cJSON *title = cJSON_GetObjectItem(json, "title");
    cJSON *description = cJSON_GetObjectItem(json, "description");

    // Check if the fields are valid strings
    if (!cJSON_IsString(author) || !cJSON_IsString(title) || !cJSON_IsString(description)) {
        printf("Invalid JSON format\n");
        return;
    }

    // Create a new JSON object without the 'type' field
    cJSON *news_item = cJSON_CreateObject();
    cJSON_AddItemToObject(news_item, "author", cJSON_Duplicate(author, 1));
    cJSON_AddItemToObject(news_item, "title", cJSON_Duplicate(title, 1));
    cJSON_AddItemToObject(news_item, "description", cJSON_Duplicate(description, 1));

    // Add the new JSON object to the global news array
    if (news_count < MAX_ITEMS) {
        news_array[news_count++] = news_item;
    } else {
        printf("News array is full\n");
        cJSON_Delete(news_item);
    }

    // Print the added news item
    char *string = cJSON_Print(news_item);
    if (string) {
        free(string);
    }
}

void command_executer(cJSON *json) {
    printf("Command Executer\n");

    cJSON *control = cJSON_GetObjectItem(json, "control");

    if (!cJSON_IsString(control) || (control->valuestring == NULL)) {
        printf("Invalid Request, control was not provided\n");
        return;
    }

    if (strcmp(control->valuestring, "buzzer") == 0) {
        cJSON *time = cJSON_GetObjectItem(json, "time");
        if (!cJSON_IsString(time) || (time->valuestring == NULL)) {
            printf("Invalid Request, time was not provided or not provided as string\n");
            return;
        }
        buzzer_time = strtoll(time->valuestring, NULL, 10);
    }

    if (strcmp(control->valuestring, "led") == 0) {
        cJSON *value = cJSON_GetObjectItem(json, "value");
        if (!cJSON_IsString(value) || (value->valuestring == NULL)) {
            printf("Invalid Request, value was not provided or not provided as string\n");
            return;
        }

        if (strcmp(value->valuestring, "on") == 0) {
            LED_Strip_Turn_ON();
            printf("LED Strip Turned ON\n");
        } else if (strcmp(value->valuestring, "off") == 0) {
            LED_Strip_Turn_OFF();
            printf("LED Strip Turned OFF\n");
        } else {
            printf("Invalid Request, value was not recognized\n");
        }
    } else {
        printf("Invalid Request, control was not recognized\n");
    }


    return;
}


void json_executer(cJSON *json){
    char* type = cJSON_GetObjectItem(json, "type")->valuestring;

    if(strcmp(type, "greeting") == 0){
        greeting_executer(json);
    } else if(strcmp(type, "data") == 0){
        data_executer(json);
    } else if(strcmp(type, "command") == 0) {
        command_executer(json);
    }  else {
        printf("Invalid command type\n");
    }

}