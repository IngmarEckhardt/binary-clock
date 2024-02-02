
#include "context.h"


void goToSleep(ClockState *clock, ButtonStates *buttons);
void wakeUp();
void incrementTimeAndUpdateLed(ClockState *clock);


ClockState watch = {0};

int main(void) {
	setupMikroController();
	setupClock(&watch);
	showHours(watch.hours);
	// change to minutes in final
	showMinutesOrSeconds(watch.seconds);

	ButtonStates buttons = {0};

	while (1) {
		
		if (watch.wakeUpFlag) {
			processUserInput(&watch, &buttons);
			
			switch (watch.state) {
				
				//intentionally fall through to hand
				case DIMMED:
				case DIMMED_SHOW_SECONDS: {
					
					handleUndimmedState(&watch);
					break;
				}
				
				case NOT_DIMMED:
				case NOT_DIMMED_SHOW_SECONDS: {
					
					handleUndimmedState(&watch);
					break;
				}
				
				case DIMMED_SET_TIME:
				case DIMMED_SET_TIME_SHOW_SECONDS:
				case NOT_DIMMED_SET_TIME:
				case NOT_DIMMED_SET_TIME_SHOW_SECONDS: {
					
					watch.state = NOT_DIMMED_SET_TIME;
					handleSetTimeMode(&watch, &buttons);
					handleUndimmedState(&watch);
					break;
				}
				
				case DIMMED_SET_TIME_HOURS_MODE:
				case DIMMED_SET_TIME_HOURS_MODE_SHOW_SECONDS:
				case NOT_DIMMED_SET_TIME_HOURS_MODE:
				case NOT_DIMMED_SET_TIME_HOURS_MODE_SHOW_SECONDS:{
					
					watch.state = NOT_DIMMED_SET_TIME_HOURS_MODE;
					handleSetTimeMode(&watch, &buttons);
					handleUndimmedState(&watch);
					break;
				}
			}
		}
		else {
			goToSleep(&watch, &buttons);
		}
	}
}

ISR(TIMER2_OVF_vect){
	incrementTimeAndUpdateLed(&watch);

	if (!watch.wakeUpFlag) {
		
		watch.wakeUpFlag = checkWakeUpButtonInterruptFlag();
		
		//wake up the AtMega if it wasn't awoke and first Button is pressed and return immediately
		if (watch.wakeUpFlag) {
			wakeUp();
		}
		
	}
	//or counting the seconds how long we are awake
	else {
		watch.awokeTimeCounterSeconds++;
	}
	
	//if we are awake for 15s without interaction we go to sleep
	if (watch.awokeTimeCounterSeconds >= AWOKE_TIME_IN_SECONDS) {
		watch.awokeTimeCounterSeconds = 0;
		watch.wakeUpFlag = FALSE;
	}
}






void goToSleep(ClockState *clock, ButtonStates *buttons) {
	
	//return if any button is still pressed
	if(anyButtonPressed(buttons)|| !allButtonAreDebounced(buttons)) {
		return;
	};
	
	turnOffLed(clock, MINUTES_LED | HOURS_LED);
	
	// Disable pull-up resistors for the buttons during sleep
	PORTD &= ~((1 << BUTTON2) | (1 << BUTTON3));
	
	clock -> state = DIMMED;
	clock -> buttonPressed = FALSE;
	
	// delete the flag for the Interrupt as last action
	EIFR &= ~(1 << INTF0);
	sleep_enable();
	sleep_mode();
}

void wakeUp() {
	sleep_disable();
	// Enable pull-up resistors for the buttons
	PORTD |= (1 << BUTTON2) | (1 << BUTTON3);
}

void incrementTimeAndUpdateLed(ClockState *clock) {
	clock -> seconds++;

	if (clock -> seconds >= SECOND_MINUTES_THRESHOLD) {
		clock -> seconds = 0;
		clock -> minutes++;
		
		
		if (clock -> minutes >= SECOND_MINUTES_THRESHOLD) {
			clock -> minutes = 0;
			clock -> hours++;

			if (clock ->hours >= HOURS_THRESHOLD) {
				clock ->hours = 0;
			}
			if (clock ->wakeUpFlag) {
				showHours(clock ->hours);
			}
		}

		if (clock ->wakeUpFlag) {
			showMinutesOrSeconds(clock ->minutes);
		}
	}
}