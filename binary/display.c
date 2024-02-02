#include "context.h"


const int LED_ON_DELAY_DIMMED_US = 50;
const int LED_OFF_DELAY_DIMMED_US = 20000;
//50% Output
const int LED_ON_DELAY_UNDIMMED_MS = 10;
const int LED_OFF_DELAY_UNDIMMED_MS = 10;

const int LED_BLINK_DELAY_MS = 1000;


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

void displayTime(ClockState *clock, uint8_t portMask) {
	clock -> ledsOn = TRUE;
	
	if (clock->state == DIMMED_SHOW_SECONDS || clock -> state == NOT_DIMMED_SHOW_SECONDS) {showMinutesOrSeconds(clock->seconds);}
	DDRC |= (portMask);
}

void turnOffLed(ClockState *clock, uint8_t portMask) {
	DDRC &= ~(portMask);
	clock -> ledsOn = FALSE;
}
void handleDimmedState(ClockState *clock) {
	
	//1% Output
	if (clock -> ledsOn && clock->quarterMs%100) {
		turnOffLed(clock, MINUTES_LED | HOURS_LED);
	} else if (!(clock->quarterMs%100) && !clock->ledsOn) {
		displayTime(clock, MINUTES_LED | HOURS_LED);
	}
}

void handleUndimmedState(ClockState *clock) {
	//50% Output, still enough for calling it undimmed
	if (clock->quarterMs%100) {
		if (clock->ledsOn) {
			turnOffLed(clock, MINUTES_LED | HOURS_LED );
		} 
		else {
			displayTime(clock, MINUTES_LED | HOURS_LED);
		}
	}
}
void blinkWithLed(ClockState *clock, uint8_t portMask) {
	//quarterMS is set to zero every second from counter2 overflow interrupt
	//reaction to value 1 if counter0 overflow interrupt already occured
	if ((clock->quarterMs == 1) && clock -> ledsOn) {
		turnOffLed(clock, portMask);
		clock -> blinkCounter++;
	}
	else if((clock->quarterMs == 1) && !(clock -> ledsOn)) {
		showMinutesOrSeconds(0xFF);
		showHours(0xFF);
		displayTime(clock, portMask);
		clock -> blinkCounter++;
	}
}