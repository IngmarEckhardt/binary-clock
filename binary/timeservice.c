#include "context.h"


void setTime(ClockState *clock,ButtonStates *buttons, volatile uint8_t *timeValue, uint8_t threshold) {
	if (clock ->state  && clock -> blinkCounter < MAX_BLINK_AMOUNT) {
		blinkWithLed(clock, HOURS_LED);
	}
	else if (clock -> blinkCounter < MAX_BLINK_AMOUNT) {
		blinkWithLed(clock, MINUTES_LED);
	}
	else {
		readAllButtons(clock, buttons);
		if(!allButtonAreDebounced(buttons)) {
			return;
		}
		if (!anyButtonPressed(buttons)) {
			clock -> buttonPressed = FALSE;
			return;
		}
		if (clock ->buttonPressed) {
			return;
		}

		if (buttons -> button2) {
			if ((*timeValue) > 0) {
				(*timeValue)--;
			}
			else {
				(*timeValue) = threshold - 1;
			}
			clock -> buttonPressed = TRUE;
		}
		
		else if (buttons -> button3) {
			(*timeValue) = ((*timeValue) + 1) % threshold;
			clock -> buttonPressed = TRUE;
		}
		
		else if (buttons -> button1) {
			clock -> buttonPressed = TRUE;
			//at first turn off setHoursMode with Button 1, then turn off setTimeMode
			if (clock -> state == NOT_DIMMED_SET_TIME_HOURS_MODE) {
				clock -> state = NOT_DIMMED_SET_TIME;
			}
			else if (clock -> state == NOT_DIMMED_SET_TIME) {
				clock -> state = NOT_DIMMED;
				} else {
				clock ->state = NOT_DIMMED_SET_TIME_HOURS_MODE;
			}
			clock -> blinkCounter = 0;
		}
		
		else if (!(buttons ->button1 || buttons ->button2 || buttons ->button3) && allButtonAreDebounced(buttons)) {
			// Reset button press flag when buttons are released
			clock -> buttonPressed = FALSE;
		}
	}
}


void handleSetTimeMode(ClockState *clock, ButtonStates *buttonStates) {
	if (clock -> state == NOT_DIMMED_SET_TIME_HOURS_MODE) {
		setTime(clock, buttonStates, &(clock ->hours), HOURS_THRESHOLD);
	}
	else if (clock -> state == NOT_DIMMED_SET_TIME) {
		setTime(clock, buttonStates,&(clock -> minutes), SECOND_MINUTES_THRESHOLD);
		clock -> seconds = 0;
		} else {
		//if we have a unexpected state, we just reset it to dimmed
		clock ->state = DIMMED;
		return;
	}
}