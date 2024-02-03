#include "context.h"



void processUserInput(ClockState *clock, ButtonState *buttons) {
	readAllButtons(buttons, 3);

	if (!anyButtonPressed(buttons, 3)) {
		clock -> state = clock -> state & ~(BUTTON_PRESSED);
		return;
	} else {
		clock->awokeTimeCounterSeconds = 0;
	}

	if (clock ->state & BUTTON_PRESSED) {
		return;
	}
	
	switch (clock -> state&MODE_BITMASK) {
		case DIMMED: {
			if (isPressed(&buttons[0])) {
				clock->state = NOT_DIMMED | BUTTON_PRESSED;
				return;
			};
			if (isPressed(&buttons[1])) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE | BUTTON_PRESSED ;
				return;
			}
			if (isPressed(&buttons[2])) {
				clock->state = DIMMED_SHOW_SECONDS | BUTTON_PRESSED;
				return;
			}
			break;
		}
		case DIMMED_SHOW_SECONDS:{
			if (isPressed(&buttons[0])) {
				clock->state = NOT_DIMMED_SHOW_SECONDS | BUTTON_PRESSED;
				return;
			};
			if (isPressed(&buttons[1])) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE |BUTTON_PRESSED;
				return;
			}
			if (isPressed(&buttons[2])) {
				clock->state = DIMMED | BUTTON_PRESSED;
				return;
			}
			break;
		}
		case NOT_DIMMED: {
			if (isPressed(&buttons[0])) {
				clock->state = clock->state | STANDBY | BUTTON_PRESSED;
				return;
			};
			if (isPressed(&buttons[1])) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE | BUTTON_PRESSED;
				return;
			}
			if (isPressed(&buttons[2])) {
				clock->state = NOT_DIMMED_SHOW_SECONDS | BUTTON_PRESSED;
				return;
			}
			break;
		}
		case NOT_DIMMED_SHOW_SECONDS: {
			if (isPressed(&buttons[0])) {
				clock->state = clock->state | STANDBY | BUTTON_PRESSED;
				return;
			};
			if (isPressed(&buttons[1])) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE | BUTTON_PRESSED;
				return;
			}
			if (isPressed(&buttons[2])) {
				clock->state = NOT_DIMMED | BUTTON_PRESSED;
				return;
			}
			break;
		}
	}
}