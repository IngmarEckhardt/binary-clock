#include "context.h"

void processUserInput(Clock *clock) {
	
	if (!(clock->state & BUTTONS)) {
		clock -> state = clock -> state & ~(BUTTON_PRESSED);
		return;
		} else {
		clock->awokeTimeCounterSeconds = 0;
	}
	
	//if still a button is pressed we return
	if (clock ->state & BUTTON_PRESSED) {
		return;
	}
	
	//the Set_Time Mode is handled in the timeService
	if (clock -> state&SET_TIME) {
		return;
	}
	//we cut away from the state-value what is not a button
	switch (clock -> state&BUTTONS) {
		case BUTTON1: {
			clock->state = STANDBY;
			break;
		}
		case BUTTON2:{
			clock->state |= SET_TIME | SET_HOUR | BUTTON_PRESSED;
			break;
		}
		//switch between minutes and seconds
		case BUTTON3: {
			clock->state ^= SECONDS | BUTTON_PRESSED;
			break;
		}
	}
}