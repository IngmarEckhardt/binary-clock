#include "context.h"



void setTime(ClockState *clock,ButtonState *buttons, volatile uint8_t *timeValue, uint8_t threshold) {

	if (!anyButtonPressed(buttons, 3)) {
		clock -> state = clock -> state & ~(BUTTON_PRESSED);
		return;
	}
	if (clock ->state & BUTTON_PRESSED) {
		return;
	}

	if (isPressed(&buttons[1])) {
		if ((*timeValue) > 0) {
			(*timeValue)--;
		}
		else {
			(*timeValue) = threshold - 1;
		}
		clock -> state = clock -> state | BUTTON_PRESSED;
	}
	
	else if (isPressed(&buttons[2])) {
		(*timeValue) = ((*timeValue) + 1) % threshold;
		clock -> state = clock -> state | BUTTON_PRESSED;
	}
	
	else if (isPressed(&buttons[0])) {
		clock -> state = clock -> state | BUTTON_PRESSED;
		//at first turn off setHoursMode with Button 1, then turn off setTimeMode
		if (clock -> state & SET_HOUR_BITMASK) {
			clock -> state = clock -> state & ~(SET_HOUR_BITMASK);
			} else {
			clock -> state = clock -> state & ~(SET_TIME_BITMASK);
		}
	}
}


void handleSetTimeMode(ClockState *clock, ButtonState *buttons) {
	if (clock -> state & SET_HOUR_BITMASK) {
		setTime(clock, buttons, &(clock ->hours), HOURS_THRESHOLD);
		} else {
		setTime(clock, buttons,&(clock -> minutes), SECOND_MINUTES_THRESHOLD);
		clock -> seconds = 0;
	}
}