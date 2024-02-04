#include "context.h"

void processUserInput(ClockState *clock) {
	readAllButtons(clock);

	if (!(clock->state & BUTTONS)) {
		clock -> state = clock -> state & ~(BUTTON_PRESSED);
		return;
	} else {
		clock->awokeTimeCounterSeconds = 0;
	}

	if (clock ->state & BUTTON_PRESSED) {
		return;
	}
	
	if (clock -> state&SET_TIME) {
		return;
	}
	switch (clock -> state&BUTTONS) {
		case BUTTON1: {
				clock->state = STANDBY;
				break;
			}
		case BUTTON2:{
					clock->state |= SET_TIME | SET_HOUR | BUTTON_PRESSED;
					break;
			}	
		case BUTTON3: {
				{
					clock->state ^= SECONDS | BUTTON_PRESSED;
					break;
				}
		}
	}
}