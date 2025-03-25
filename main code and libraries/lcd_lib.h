#define LCD_DATA PORTC
#define ctrl PORTB
#define rs PINB7
#define rw PINB6
#define en PINB5

#include <avr/io.h>
#include <util/delay.h>

void LCD_cmd(unsigned char);
void LCD4_cmd(unsigned char);
void LCD_init();
void LCD4_init();
void LCD_write(unsigned char);
void LCD4_write(unsigned char);
void LCD_write_string(unsigned char*);
void LCD4_write_string(unsigned char*);
void LCD_gotoxy(int, int);
void LCD4_gotoxy(int, int);
void LCD_clear();
void LCD4_clear();