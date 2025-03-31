# DHT22-AVR-Microcontroller

Attention:

-The file includes both DHT library and 16x2 LCD library

-So apparently there is a bug with sprintf displaying float values, it will print out garbage values on lcd screen. (fixed)

->I have a workaround with it, by forcing it to display the temperature value in integer (eg. 27.3C will be displayed as 273 C), things seem to work fine with this one. 

->possible culprit may be: "Your compiler’s sprintf() implementation might not support floating-point formatting (%.1f) due to a minimal libc configuration (common in embedded systems like AVR-GCC to save space)." (fixed)

->Displaying floating-values issues can be fixed by tweaking some settings inside the AVR-GCC compiler (Mine is Atmel Studio 7) - Illustrated in Figure 1 and 2

-LEDs depend on Temp range have been updated.

- UART to display on Hyperterminal (Updated)

1st/Aprl/2025
