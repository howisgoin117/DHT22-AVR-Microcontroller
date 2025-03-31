/*
#define F_CPU 1000000UL
#include <util/delay.h>
#include "DHT.h"
#include "lcd_lib.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED PD3    // Red LED on PORTD, pin 3
#define GREEN_LED PD4  // pin 4
#define YELLOW_LED PD5 // pin 5


void init_ports() {
	DDRD |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED);
	PORTD &= ~((1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED));
}

//conditions for turning LEDs on and off
void control_led(double temp) {
	if (temp < 24.0) {
		PORTD |= (1 << RED_LED);
		} else {
		PORTD &= ~(1 << RED_LED);
	}
	if (temp >= 24.0 && temp <= 26.0) {
		PORTD |= (1 << YELLOW_LED);
		} else {
		PORTD &= ~(1 << YELLOW_LED);
	}
	if (temp > 26.0) {
		PORTD |= (1 << GREEN_LED);
		} else {
		PORTD &= ~(1 << GREEN_LED);
	}
}


int main(void) {

	DDRC = 0xFF;
	DDRB = 0xFF;
	init_ports();
	LCD4_init();
	DHT_Setup();
	
	while (1) {
		char buffer[32];
		double temperature;
		double humidity;
		
		DHT_Read(&temperature, &humidity);
		control_led(temperature);
		
		LCD4_gotoxy(1, 1);
		LCD4_write_string("Temperature:");
		LCD4_gotoxy(2, 1);

		switch (DHT_GetStatus()) {
			case DHT_Ok:
			sprintf(buffer, "%d C", (int)(temperature * 10));
			LCD4_write_string((unsigned char*)buffer);
			break;
			case DHT_Error_Checksum:
			LCD4_write_string("C Error");
			break;
			case DHT_Error_Timeout:
			LCD4_write_string("T Error");
			break;
			case DHT_Error_Temperature:
			LCD4_write_string("TR Error");
			break;
			default:
			LCD4_write_string("Unknown Err");
			break;
		}
		_delay_ms(2000);
	}
	return 0;
}


//test LCD and LEDs
void init_ports() {
	DDRD |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED);
	PORTD &= ~((1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED));
}
int main(void) {
	DDRC = 0xFF;
	DDRB = 0xFF;
	LCD_init();
	LCD_clear();
	LCD_gotoxy(1, 1);
	LCD_write_string((unsigned char *)"Test LCD");
	init_ports();
	PORTD |= (1 << RED_LED);
	PORTD |= (1 << GREEN_LED);
	PORTD |= (1 << YELLOW_LED);

	while (1) {
		// Loop forever
	}
}

*/

//Updated code with UART 
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define F_CPU 1000000UL  //1 MHz clock
#define BAUD 4800        
#define MYUBRR ((F_CPU / (16UL * BAUD)) - 1)  //calculation for 4800 baud

#include <util/delay.h>
#include "DHT.h"
#include "lcd_lib.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED PD3    // Red LED on PORTD, pin 3
#define GREEN_LED PD4  // pin 4
#define YELLOW_LED PD5 // pin 5

// UART initialization function for ATmega16A
void UART_init(unsigned int ubrr) {
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1 << RXEN) | (1 << TXEN);  // Enable RX and TX
	UCSRC = (1 << URSEL) | (3 << UCSZ0); // 8-bit data, 1 stop bit
}

// UART transmit function
void UART_transmit(unsigned char data) {
	while (!(UCSRA & (1 << UDRE)));  // Wait for empty transmit buffer
	UDR = data;                      // Send data
}

// Transmit a string via UART
void UART_transmit_string(char* str) {
	while (*str) {
		UART_transmit(*str++);
	}
}

void init_ports() {
	DDRD |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED);
	PORTD &= ~((1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED));
}

void control_led(double temp) {
	if (temp < 24.0) {
		PORTD |= (1 << RED_LED);
		} else {
		PORTD &= ~(1 << RED_LED);
	}
	if (temp >= 24.0 && temp <= 26.0) {
		PORTD |= (1 << YELLOW_LED);
		} else {
		PORTD &= ~(1 << YELLOW_LED);
	}
	if (temp > 26.0) {
		PORTD |= (1 << GREEN_LED);
		} else {
		PORTD &= ~(1 << GREEN_LED);
	}
}

int main(void) {
	DDRC = 0xFF;
	DDRB = 0xFF;
	init_ports();
	LCD4_init();
	DHT_Setup();
	UART_init(MYUBRR);

	// Send test message at startup
	UART_transmit_string("System Started - ATmega16A @ 1MHz\r\n");

	while (1) {
		char buffer[32];
		double temperature;
		double humidity;
		
		DHT_Read(&temperature, &humidity);
		control_led(temperature);
		
		LCD4_gotoxy(1, 1);
		LCD4_write_string("Temperature:");
		LCD4_gotoxy(2, 1);

		switch (DHT_GetStatus()) {
			case DHT_Ok:
			sprintf(buffer, "%.1f C", temperature);
			LCD4_write_string((unsigned char*)buffer);
			
			sprintf(buffer, "Temp: %.1f C, Hum: %.1f%%\r\n", temperature, humidity);
			UART_transmit_string(buffer);
			break;
			case DHT_Error_Checksum:
			LCD4_write_string("C Error");
			UART_transmit_string("Checksum Error\r\n");
			break;
			case DHT_Error_Timeout:
			LCD4_write_string("T Error");
			UART_transmit_string("Timeout Error\r\n");
			break;
			case DHT_Error_Temperature:
			LCD4_write_string("TR Error");
			UART_transmit_string("Temperature Read Error\r\n");
			break;
			default:
			LCD4_write_string("Unknown Err");
			UART_transmit_string("Unknown Error\r\n");
			break;
		}
		_delay_ms(2000);
	}
	return 0;
}
