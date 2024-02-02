#include "context.h"

void processUserInput(ClockState *clock, ButtonStates *buttons) {
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
	
	switch (clock -> state) {
		case DIMMED: {
			if (buttons->button1) {
				clock->state = NOT_DIMMED;
				clock->buttonPressed = TRUE;
				return;
			};
			if (buttons->button2) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE;
				clock->buttonPressed = TRUE;
				return;
			}
			if (buttons->button3) {
				clock->state = DIMMED_SHOW_SECONDS;
				clock->buttonPressed = TRUE;
				return;
			}
			break;
		}
		case DIMMED_SHOW_SECONDS:{
			if (buttons->button1) {
				clock->state = NOT_DIMMED_SHOW_SECONDS;
				clock->buttonPressed = TRUE;
				return;
			};
			if (buttons->button2) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE;
				clock->buttonPressed = TRUE;
				return;
			}
			if (buttons->button3) {
				clock->state = DIMMED;
				clock->buttonPressed = TRUE;
				return;
			}
			break;
		}
		case NOT_DIMMED: {
			if (buttons->button1) {
				clock->wakeUpFlag = FALSE;
				clock->buttonPressed = TRUE;
				return;
			};
			if (buttons->button2) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE;
				clock->buttonPressed = TRUE;
				return;
			}
			if (buttons->button3) {
				clock->state = NOT_DIMMED_SHOW_SECONDS;
				clock->buttonPressed = TRUE;
				return;
			}
			break;
		}
		case NOT_DIMMED_SHOW_SECONDS: {
			if (buttons->button1) {
				clock->wakeUpFlag = FALSE;
				clock->buttonPressed = TRUE;
				return;
			};
			if (buttons->button2) {
				clock->state = NOT_DIMMED_SET_TIME_HOURS_MODE;
				clock->buttonPressed = TRUE;
				return;
			}
			if (buttons->button3) {
				clock->state = NOT_DIMMED;
				clock->buttonPressed = TRUE;
				return;
			}
			break;
		}
	}
}