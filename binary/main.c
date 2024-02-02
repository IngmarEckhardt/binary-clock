#include "context.h"

//we send the controller to idle until counter0 overflow
void goToIdle(ClockState *clock, ButtonStates *buttons);
//we turn off pullups and counter0 with its interrupts
void goToStandby(ClockState *clock, ButtonStates *buttons);
//we wake up from standby, turn on pull-ups and prepare Idle-Mode and counter 0 with its interrupts
void wakeUpFromStandby();
//every second the time and ports for led are updated from counter2 overflow interrupt
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
					
					handleDimmedState(&watch);
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
			goToIdle(&watch,&buttons);
		}
		else {
			goToStandby(&watch, &buttons);
		}
	}
}

ISR(TIMER2_OVF_vect){
	incrementTimeAndUpdateLed(&watch);

	if (!watch.wakeUpFlag) {
		
		watch.wakeUpFlag = checkWakeUpButtonInterruptFlag();
		
	
		if (watch.wakeUpFlag) {
			wakeUpFromStandby();
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

ISR(TIMER0_OVF_vect){
	watch.quarterMs++;
}

void goToIdle(ClockState *clock, ButtonStates *buttons) {
		
		//return if any button is still pressed
		if(anyButtonPressed(buttons)|| !allButtonAreDebounced(buttons)) {
			return;
		};
		
		//sleep mode control bits all zero for idle, just enable sleep
		SMCR = 1;
		sleep_mode();
}


void goToStandby(ClockState *clock, ButtonStates *buttons) {
	
	//return if any button is still pressed
	if(anyButtonPressed(buttons)|| !allButtonAreDebounced(buttons)) {
		return;
	};
	//Overflow-Interrupt for Counter0 off
	TIMSK0 &= ~(1 << TOIE2);
	//turn off counter0
	PRR &= ~(1 <<   PRTIM0);
	turnOffLed(clock, MINUTES_LED | HOURS_LED);
	
	// Disable pull-up resistors for the buttons during sleep
	PORTD &= ~((1 << BUTTON2) | (1 << BUTTON3));
	
	clock -> state = DIMMED;
	clock -> buttonPressed = FALSE;
	
	// delete the flag for the Interrupt as last action
	EIFR &= ~(1 << INTF0);
	//sleep mode with counter2 still active (Extended Standby) and Enable Sleep
	SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE);
	sleep_mode();
}

void wakeUpFromStandby() {
	sleep_disable();
	// Enable pull-up resistors for the buttons
	PORTD |= (1 << BUTTON2) | (1 << BUTTON3);
	//pre scaler 256 for counter 0 -> ca 0,25ms with 1Mhz CPU Clock
	TCCR0B |= (1 << CS22);
	//Overflow-Interrupt for Counter0 on
	TIMSK0 |= (1 << TOIE2);
	//turn on timer
	PRR |= (1 <<   PRTIM0);

}

void incrementTimeAndUpdateLed(ClockState *clock) {
	clock -> quarterMs = 0;
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