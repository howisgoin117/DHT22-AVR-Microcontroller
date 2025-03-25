#include "DHT.h"
#include "lcd_lib.h"
#include <stdio.h>  
#define F_CPU 8000000UL  
#include <util/delay.h>

int main(void)
{

	DDRC = 0xFF;
	DDRB = 0xFF;
	
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
