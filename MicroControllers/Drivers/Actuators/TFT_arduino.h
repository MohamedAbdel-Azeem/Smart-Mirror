#ifndef TFT_arduino_h
#define TFT_arduino_h

#include <stdint.h> // For standard integer types


// Pin definitions for Arduino
// #define LCD_RD   27
// #define LCD_WR   28   
// #define LCD_RS   29        
// #define LCD_CS   12      
// #define LCD_REST 13

// // Data pins
// #define LCD_D0   25
// #define LCD_D1   15
// #define LCD_D2   16
// #define LCD_D3   17
// #define LCD_D4   18
// #define LCD_D5   19
// #define LCD_D6   20
// #define LCD_D7   21



// Pin definitions for Pico
// //TFT 1
// #define LCD_RD   22
// #define LCD_WR   26   
// #define LCD_RS   27        
// #define LCD_CS   28      
// #define LCD_REST 21

// // Data pins
// #define LCD_D0   2
// #define LCD_D1   3
// #define LCD_D2   4
// #define LCD_D3   5
// #define LCD_D4   6
// #define LCD_D5   7
// #define LCD_D6   8
// #define LCD_D7   9



// TFT 2
// #define LCD_RD_T2   22
// #define LCD_WR_T2   18   
// #define LCD_RS_T2   19        
// #define LCD_CS_T2   28      
// #define LCD_REST_T2 20

// // Data pins
// #define LCD_D0_T2   10
// #define LCD_D1_T2   11
// #define LCD_D2_T2   12
// #define LCD_D3_T2   13
// #define LCD_D4_T2   14
// #define LCD_D5_T2   15
// #define LCD_D6_T2   16
// #define LCD_D7_T2   17


extern uint LCD_WR;
extern uint LCD_RS;
extern uint LCD_CS;
extern uint LCD_REST;
extern uint LCD_D0;
extern uint LCD_D1;
extern uint LCD_D2;
extern uint LCD_D3;
extern uint LCD_D4;
extern uint LCD_D5;
extern uint LCD_D6;
extern uint LCD_D7;


void TFT_init( int tft);
void LCD_Draw_String( unsigned int x, unsigned int y, const char *str, unsigned int color, float scale, int tft);
void Lcd_Clear(unsigned int color, int tft);
void UpdateDisplay(char* prev_str, char* new_str, unsigned int x, unsigned int y, uint16_t textColor, uint16_t bgColor,float scale, int tft) ;
void LCD_Draw_String_vertical(unsigned int x, unsigned int y, const char *str, unsigned int color, float scale, int tft);


// void LCD_Draw_Vertical_String(unsigned int x, unsigned int y, const char *str, unsigned int color, float scale);


#endif