// Flat C syncronous functions to use i2c on the atmel 328p MCU
#include <stdint.h>
void twiInit(); // Sets SCL to 400kHz (max for 328p)
void twiStart();
void twiStop();
void twiWrite(uint8_t data);
uint8_t twiReadAck();
uint8_t twiReadNack();
uint8_t twiGetStatus();


