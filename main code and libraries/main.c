#define F_CPU 8000000UL  

#include <util/delay.h>
#include "DHT.h"
#include "lcd_lib.h"
#include <stdio.h>  

#define RED_LED PD0  // Red LED on PORTB, pin 0
#define GREEN_LED PD1 //pin 1
#define	YELLOW_LED PD2 //pin 2



void init_ports() {
	// Set PB0,1,2 (LEDs) as output
	DDRD |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED);
	// Initially turn off the LEDs
	PORTD &= ~(1 << RED_LED) | (1 << GREEN_LED) | (1 << YELLOW_LED);
	
	
}

//Define controlled temp is between 24 and 26 C
void control_led(double temp) {
	if (temp < 24.0) {
		PORTD |= (1 << RED_LED);   // Turn ON red LED
		} else {
		PORTD &= ~(1 << RED_LED);  // Turn OFF red LED
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

int main(void)
{

	DDRC = 0xFF;
	DDRB = 0xFF;
	init_ports();        // LED pin setup
	
	// Initialize once before loop
	LCD4_init();  // Initialize LCD
	DHT_Setup();  // Initialize DHT sensor
	
	while (1)
	{
		char buffer[16];     
		double temperature;  // Scalar, not array
		double humidity;     // Scalar, not array
		
		// Read from sensor
		DHT_Read(&temperature, &humidity);  // Pass addresses of scalars
		
		// Control LED based on temperature
		control_led(temperature);
		
		LCD4_gotoxy(1, 1);  // Row 1, Column 1
		LCD4_write_string("Temperature:");
		
		LCD4_gotoxy(2, 1);  // Row 2, Column 1

		// Check status
		switch (DHT_GetStatus())
		{
			case DHT_Ok:
			// Print temperature
			sprintf(buffer, "%d C", (int)(temperature * 10));  // Use scalar value
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
		
		// Wait for sensor to stabilize
		_delay_ms(2000);
	}
	
	return 0;
}


/*
//test LCD
int main(void) {
	DDRC = 0xFF;
	DDRB = 0xFF;
	LCD_init();
	LCD_clear();
	LCD_gotoxy(1, 1);
	LCD_write_string((unsigned char *)"Test LCD");

	while (1) {
		// Loop forever
	}
}

*/
