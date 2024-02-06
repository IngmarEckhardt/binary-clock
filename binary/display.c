#include "context.h"

//set the ports to the LED according the bit-Values of the parameter
void showMinutesOrSeconds(uint8_t value) {
	PORTD = (PORTD & 0b11111100) | ((value & 0b01) << 1) | ((value & 0b10) >> 1);
	PORTC = (PORTC & 0b11011000) | ((value & 0b100000) >> 5) | ((value & 0b10000) >> 3) | ((value & 0b1000) >> 1) | ((value & 0b100) << 3);
	
}

void showHours(uint8_t value) {
	PORTD = (PORTD & 0b10011111) | (value << 5);
	PORTB = (PORTB & 0b11111000) | (value >> 2);
}

//define the ports as output, drives them as ground
void displayTime(uint8_t portMask) {
	DDRC |= (portMask);
}

//define them as input, turn off led
void turnOffLed(uint8_t portMask) {
	DDRC &= ~(portMask);
}
void handleDisplay(uint8_t idleCounter) {
	//turn on led only if idle counter has the overflow 1/256 -> 0,4% Output
	if (idleCounter) {
		turnOffLed(MINUTES_LED | HOURS_LED);
		} else {
		displayTime(MINUTES_LED | HOURS_LED);
	}
}