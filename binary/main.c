#include "context.h"

//we send the controller to idle until counter0 interrupt occur
void goToIdle();
//we turn off pull up resistor and counter0 with its interrupts
void goToStandby(ClockState *clock);
//we wake up from standby, turn on pull-ups and prepare Idle-Mode and counter 0 with its interrupts
void wakeUpFromStandby(ClockState *clock);
//every second the time and ports for led are updated from counter2 overflow interrupt
void incrementTimeAndUpdateLed(ClockState *clock);
//we just set the correct value for the ports of the led, but they are driven by Port C3 and Port C4 as ground and dark without these ports correctly set
void calculateAndSetLedForTime(ClockState *clock);

ClockState watch = {0};

int main(void) {
	
	setupMikroController();
	setupClock(&watch);
	
	while (1) {
		if (watch.state & STANDBY) {
			goToStandby(&watch);
			} else {
			goToIdle();
		}
	}
}

ISR(TIMER2_OVF_vect){
	//turn off interrupt for counter 0 during routine
	TIMSK0 &= ~(1 << OCIE0A);
	
	incrementTimeAndUpdateLed(&watch);
	
	//all Buttons can wake us up from standby
	if (watch.state&STANDBY) {
		PORTD |= BUTTON1;
		readAllButtons(&watch);
		
		if (watch.state&BUTTON1) {
			wakeUpFromStandby(&watch);
			} else {
			PORTD &= ~(BUTTON1);
		}
		} else {
		//or counting the seconds how long we are awake
		watch.awokeTimeCounterSeconds++;
		calculateAndSetLedForTime(&watch);
		
	}
	

	//if we are awake for 15s without interaction we go to sleep
	if (watch.awokeTimeCounterSeconds >= AWOKE_TIME_IN_SECONDS) {
		watch.state = STANDBY;
	}
	
	//turn on interrupt for counter 0 after routine
	if (!(watch.state&STANDBY)) {
		TIFR0 &= ~(1<<OCIE0A);
		TIMSK0 |= (1 << OCIE0A);
	}
}


ISR(TIMER0_COMPA_vect){
	watch.idleCounter++;
	
	if (!watch.idleCounter) {
		readAllButtons(&watch);
		processUserInput(&watch);
		
		if (watch.state&SET_TIME) {
			watch.state &= ~(SECONDS);
			handleSetTimeMode(&watch);
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

void goToStandby(ClockState *clock) {
	//counter 0 overflow interrupt off
	TIMSK0 &= ~(1 << OCIE0A);
	
	turnOffLed(MINUTES_LED | HOURS_LED);
	
	readAllButtons(clock);
	if (clock->state&BUTTON1) {
		//counter 0 overflow interrupt on
		TIMSK0 |= (1 << OCIE0A);
		return;
	}
	//turn off pull ups
	PORTD &= ~(BUTTON1| BUTTON2 | BUTTON3);
	
	//sleep mode with counter2 still active (Extended Standby)
	SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE);
	
	sleep_mode();
}

void wakeUpFromStandby(ClockState *clock) {
	//Set Sleep-Mode To Idle and Sleep disabled
	SMCR = 0;
	
	clock -> awokeTimeCounterSeconds = 0;
	
	// To-Do: Change to Minutes in final version, Button pressed because we get awoke with a button
	clock -> state = BUTTON_PRESSED;
	
	//Turn On Pull-Ups-after Sleep
	PORTD |= BUTTON1| BUTTON2 | BUTTON3;
	
	calculateAndSetLedForTime(clock);
	//delete counter interrupt
	TIFR0 &= ~(1 << OCF0A);
	//Overflow-Interrupt for Counter0 on
	TIMSK0 |= (1 << OCIE0A);
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
void readAllButtons(ClockState *clock) {
	//we cut away button values from old state, and place the button-values (opposite of the port because of pull-ups) from the ports there
	clock -> state = (clock -> state & ~(BUTTONS)) | (~PIND & BUTTONS);
}

void calculateAndSetLedForTime(ClockState *clock)
{
	showHours(clock ->hours);
	if (clock -> state & SECONDS) {
		showMinutesOrSeconds(watch.seconds);
		} else {
		showMinutesOrSeconds(watch.minutes);
	}
}