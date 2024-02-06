#include "context.h"

//we turn off pull up resistor and counter0 with its interrupts
void goToStandby(Clock *clock);
//we wake up from standby, turn on pull-ups and prepare Idle-Mode and counter 0 with its interrupts
void wakeUpFromStandby(Clock *clock);
//every second the time are updated from counter2 overflow interrupt
void incrementTime(Clock *clock);
//we just set the correct value for the ports of the led, but they are driven by Port C3 and Port C4 as ground and dark without these ports correctly set
void calculateAndSetLedForTime(Clock *clock);
void clearCounter0InterruptFlagAndTurnOnInterrupt();

Clock watch = {0};

int main(void) {
	
	setupMikroController();
	setupClock(&watch);
	
	while (1) {
		sleep_mode();
	}
}

ISR(TIMER2_OVF_vect){
	//turn off interrupt for counter 0 during routine
	TIMSK0 &= ~(1 << OCIE0A);
	//Turn on pull up
	PORTD |= BUTTON1;
	
	incrementTime(&watch);
	
	//all Buttons can wake us up from standby
	if (watch.state&STANDBY) {
		
		readAllButtons(&watch);
		
		if (watch.state&BUTTON1) {
			watch.state &= ~STANDBY;
			wakeUpFromStandby(&watch);
		}
		//turn off pull up if we stay in stand-by
		PORTD &= ~(BUTTON1);
	} 
	else {
		//or counting the seconds how long we are awake
		watch.awokeTimeCounterSeconds++;
		calculateAndSetLedForTime(&watch);
		
		//if we are awake for 15s without interaction we go to sleep
		if (watch.awokeTimeCounterSeconds >= AWOKE_TIME_IN_SECONDS) {
			watch.state = STANDBY;
			return;
		}
		clearCounter0InterruptFlagAndTurnOnInterrupt();
		return; 
	}	
}

ISR(TIMER0_COMPA_vect){
	
	watch.idleCounter++;
	
	//its still enough to read a few dozen times per minute, read onlye if idle counter has a overflow to zero
	if (!watch.idleCounter) {
		readAllButtons(&watch);
		processUserInput(&watch);
		
		if (watch.state&STANDBY) {
			goToStandby(&watch);
			return;
		}
		
		if (watch.state&SET_TIME) {
			watch.state &= ~(SECONDS);
			handleSetTimeMode(&watch);
		}
	}
	//turn on led only if idle counter has the overflow 1/256 -> 0,4% Output
	handleDisplay(watch.idleCounter);
}

void goToStandby(Clock *clock) {
	//counter 0 overflow interrupt off
	TIMSK0 &= ~(1 << OCIE0A);
	
	turnOffLed(MINUTES_LED | HOURS_LED);
	
	readAllButtons(clock);
	//return if Button1 is still pressed
	if (clock->state&BUTTON1) {
		//counter 0 overflow interrupt on
		TIMSK0 |= (1 << OCIE0A);
		return;
	}
	//turn off pull ups
	PORTD &= ~(BUTTON1 | BUTTON2 | BUTTON3);
	//sleep mode with counter2 still active (External Standby)
	SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0);
}

void wakeUpFromStandby(Clock *clock) {
	//Set Sleep-Mode To Idle and Sleep disabled
	SMCR = 0;
	
	clock -> awokeTimeCounterSeconds = 0;
	
	//Button pressed because we get awoke with a button
	clock -> state = BUTTON_PRESSED;
	
	//Turn On Pull-Ups
	PORTD |= BUTTON1 | BUTTON2 | BUTTON3;
	
	calculateAndSetLedForTime(clock);
	clearCounter0InterruptFlagAndTurnOnInterrupt();
}

void incrementTime(Clock *clock) {
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

void readAllButtons(Clock *clock) {
	//we cut away button values from old state, and place the button-values (opposite of the port because of pull-ups) from the ports there
	clock -> state = (clock -> state & ~(BUTTONS)) | (~PIND & BUTTONS);
}

void calculateAndSetLedForTime(Clock *clock) {
	showHours(clock ->hours);
	if (clock -> state & SECONDS) {
		showMinutesOrSeconds(clock->seconds);
		} else {
		showMinutesOrSeconds(clock->minutes);
	}
}
void clearCounter0InterruptFlagAndTurnOnInterrupt()
{
	TIFR0 &= ~(1<<OCIE0A);
	TIMSK0 |= (1 << OCIE0A);
}