#include "context.h"

void setTime(Clock *clock, volatile uint8_t *timeValue, uint8_t threshold) {

	if (!(clock->state & BUTTONS)) {
		clock -> state = clock -> state & ~(BUTTON_PRESSED);
		return;
		} else {
		clock->awokeTimeCounterSeconds = 0;
	}
	if (clock ->state & BUTTON_PRESSED) {
		return;
	}
	//we cut away from the state-value what is not a button
	switch (clock -> state&BUTTONS) {
		case BUTTON1: {
			if ((*timeValue) > 0) {
				(*timeValue)--;
			}
			else {
				(*timeValue) = threshold - 1;
			}
			clock -> state |= BUTTON_PRESSED;
			break;
		}
		case BUTTON2:{
			clock -> seconds = 0;
			if (clock -> state & SET_HOUR) {
				clock->state &= ~SET_HOUR;
				clock->state |= BUTTON_PRESSED;
				} else {
				clock->state &= ~SET_TIME;
				clock->state |= BUTTON_PRESSED;
			}
			break;
		}
		case BUTTON3: {
			(*timeValue) = ((*timeValue) + 1) % threshold;
			clock -> state |= BUTTON_PRESSED;
			break;
		}
	}
}


void handleSetTimeMode(Clock *clock) {

	if (clock->state & SET_HOUR) {
		setTime(clock, &(clock->hours), HOURS_THRESHOLD);
		} else {
		setTime(clock, &(clock->minutes), SECOND_MINUTES_THRESHOLD);
	}
	
}