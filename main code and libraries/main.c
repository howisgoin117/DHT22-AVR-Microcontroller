//Headers

#define F_CPU 8000000UL //define microcontroller clock speed

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd_lib.h"
#include "DHT.h"

int main(void)
{
	
	//Loop
	while (1 == 1)
	{
		
		char buffer[16];  // Buffer for LCD display
		double temperature[1];
		double humidity[1];
		
		DDRC = 0xFF;
		DDRB = 0xFF;
		//Read from sensor

		
		LCD4_init();  // Initialize LCD
		DHT_Setup(); // Initialize DHT sensor
		
		//Read from sensor
		DHT_Read(temperature, humidity);
		
		
		LCD4_gotoxy(1, 1);  // Row 1, Column 1
		LCD4_write_string("Temperature:");
		
		LCD4_gotoxy(2, 1); // row 2, column 1

		//Check status
		switch (DHT_GetStatus())
		{
			case (DHT_Ok):
			//Print temperature
			//print(temperature[0]);
			sprintf(buffer, "%.1f C", temperature);
			LCD4_write_string((unsigned char*)buffer);
			break;
			case (DHT_Error_Checksum):
			LCD4_write_string("C Error");
			break;
			case (DHT_Error_Timeout):
			LCD4_write_string("T Error");
			break;
			case DHT_Error_Temperature:
			LCD4_write_string("TR Error");
			break;
		}
		
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(1000);
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
