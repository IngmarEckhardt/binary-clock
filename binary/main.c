#include "context.h"


//we send the controller to idle until counter0 overflow
void goToIdle();
//we turn off pull up resistor and counter0 with its interrupts
void goToStandby(ClockState *clock, ButtonState *button);
//we wake up from standby, turn on pull-ups and prepare Idle-Mode and counter 0 with its interrupts
void wakeUpFromStandby(ClockState *clock);
//every second the time and ports for led are updated from counter2 overflow interrupt
void incrementTimeAndUpdateLed(ClockState *clock);

ClockState watch = {0};
ButtonState buttons[3];

int main(void) {
	setupMikroController();
	setupClock(&watch, buttons);
	while (1) {
		if (watch.state & STANDBY) {
			goToStandby(&watch, &buttons[0]);
			} else {
			goToIdle();
		}
	}
}

ISR(TIMER2_OVF_vect){
	//turn off interrupt for counter 0 during routine
	TIMSK0 &= ~(1 << TOIE0);
	
	incrementTimeAndUpdateLed(&watch);
	
	if (watch.state&STANDBY) {
		
		readButton(&buttons[0]);
		
		if (buttons[0].button_changeLog&TRUE) {
			wakeUpFromStandby(&watch);
		}
	} else {
		//or counting the seconds how long we are awake
		watch.awokeTimeCounterSeconds++;
		showHours(watch.hours);
		if (watch.state & SECONDS_MODE_BITMASK) {
			showMinutesOrSeconds(watch.seconds);
			} else {
			showMinutesOrSeconds(watch.minutes);
		}
	}

	//if we are awake for 15s without interaction we go to sleep
	if (watch.awokeTimeCounterSeconds >= AWOKE_TIME_IN_SECONDS) {
		watch.state = DIMMED | STANDBY;
	}
	//turn on interrupt for counter 0 after routine
	if (!(watch.state&STANDBY)) {
		TIFR0 &= ~(1<<TOV0);
		TIMSK0 |= (1 << TOIE0);
	}
}


ISR(TIMER0_OVF_vect){
	watch.idleCounter++;
	
	if (!watch.idleCounter%32) {
		processUserInput(&watch, buttons);
		if (watch.state&SET_TIME_BITMASK) {
			watch.state = watch.state & ~(SECONDS_MODE_BITMASK) & ~(DIMMED);
			handleSetTimeMode(&watch, buttons);
		}
	}
	handleDisplay(watch.idleCounter);
}

void goToIdle() {
	//sleep mode control bits all zero for idle, just enable sleep
	SMCR = 1;
	sleep_mode();
	SMCR = 0;
}


void goToStandby(ClockState *clock, ButtonState *button) {
	//counter 0 overflow interrupt off
	TIMSK0 &= ~(1 << TOIE0);
	
	if (isPressed(button)) {
		//counter 0 overflow interrupt on
		TIMSK0 |= (1 << TOIE0);
		return;
	}
		
	turnOffLed(MINUTES_LED | HOURS_LED);
	
	//sleep mode with counter2 still active (Extended Standby)
	SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE);
	sleep_mode();
}

void wakeUpFromStandby(ClockState *clock) {
	//Set Sleep-Mode To Idle and Sleep disabled
	SMCR = 0;
	
	clock -> awokeTimeCounterSeconds = 0;
	clock -> state = DIMMED_SHOW_SECONDS | BUTTON_PRESSED;
	
	TIFR0 &= ~(1<<TOV0);
	//Overflow-Interrupt for Counter0 on
	TIMSK0 |= (1 << TOIE2);
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
		}
	}
}