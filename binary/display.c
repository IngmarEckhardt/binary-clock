#include "context.h"

//set the ports to the LED according the bit-Values of the parameter
void showMinutesOrSeconds(uint8_t value) {
	if ((value >> 0) & 0x01) {PORTD |= (1 << PD1);}
	else {PORTD &= ~(1 << PD1);}
	
	if ((value >> 1) & 0x01) {PORTD |= (1 << PD0);}
	else {PORTD &= ~(1 << PD0);}
	
	if ((value >> 2) & 0x01) {PORTC |= (1 << PC5);}
	else {PORTC &= ~(1 << PC5);}
	
	if ((value >> 3) & 0x01) {PORTC |= (1 << PC2);}
	else {PORTC &= ~(1 << PC2);}
	
	if ((value >> 4) & 0x01) {PORTC |= (1 << PC1);}
	else {PORTC &= ~(1 << PC1);}
	
	if ((value >> 5) & 0x01) {PORTC |= (1 << PC0);}
	else {PORTC &= ~(1 << PC0);}
	
}

void showHours(uint8_t value) {
	if ((value >> 0) & 0x01) {PORTD |= (1 << PD5);}
	else {PORTD &= ~(1 << PD5);}
	
	if ((value >> 1) & 0x01) {PORTD |= (1 << PD6);}
	else {PORTD &= ~(1 << PD6);}
	
	if ((value >> 2) & 0x01) {PORTB |= (1 << PB0);}
	else {PORTB &= ~(1 << PB0);}
	
	if ((value >> 3) & 0x01) {PORTB |= (1 << PB1);}
	else {PORTB &= ~(1 << PB1);}
	
	if ((value >> 4) & 0x01) {PORTB |= (1 << PB2);}
	else {PORTB &= ~(1 << PB2);}
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