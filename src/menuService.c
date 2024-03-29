#include "context.h"

void processUserInput(Clock *clock) {
	
	if (!(clock->state & BUTTONS)) {
		//if no button is pressed we can clear the bit if necessary and return
		clock -> state &= ~(BUTTON_PRESSED);
		return;
		} else {
		//or reset the counter if a user interaction is detected	
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
		//go to Set Time, start with hours
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