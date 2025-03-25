#ifndef F_CPU
#define F_CPU 8000000UL //define microcontroller clock speed
#endif


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd_lib.h"
#include "DHT.h"

uint16_t temperature_int = 0;
uint16_t humidity_int = 0;


int main(void)
{
	DDRC = 0xFF;
	DDRB = 0xFF;
	LCD4_init();  // Initialize LCD

	while (1) {
		Display_DHT22_Data();
		_delay_ms(1000);  // Wait before the next reading
	}
}
