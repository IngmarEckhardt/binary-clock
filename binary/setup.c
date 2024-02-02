#include "context.h"


const int HOURS_THRESHOLD = 24;
const int SECOND_MINUTES_THRESHOLD = 60;
//auto sleep after 15s
const int AWOKE_TIME_IN_SECONDS = 15;
//doubled value for 3 blinks
const int MAX_BLINK_AMOUNT = 6;


void setupClock(ClockState *clock) {
	// To-Do: Change in final to Dimmed with Minutes
	clock->state = DIMMED_SHOW_SECONDS;
	clock->wakeUpFlag = TRUE;
	clock->ledsOn = FALSE;
	clock->buttonPressed = FALSE;
	
	clock->hours = 13;
	clock->minutes = 50;
	clock->seconds = 50;
	clock->quarterMs = 0;
	clock->blinkCounter = 0;
	clock->awokeTimeCounterSeconds = 0;
}

void setupMikroController() {
	//set Data-Direction Output for LED
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD5) | (1 << DDD6);

	//Input for Buttons
	DDRD &= ~(1 << BUTTON1);
	DDRD &= ~(1 << BUTTON2);
	DDRD &= ~(1 << BUTTON3);


	//Using asynchronous timer with low frequency crystal
	ASSR |= (1 << AS2);

	//pre scaler 128 (256x128 = 32768))
	TCCR2B |= (1 << CS22) | (1 << CS20);

	//Overflow-Counter Interrupt on
	TIMSK2 |= (1 << TOIE2);

	//pull up for Button 1 always on
	PORTD |= (1 << BUTTON1) | (1 << BUTTON2) | (1 << BUTTON3);

	// trigger button 1 INT0 interrupt recognition on any change
	MCUCR |= (1 << ISC01);

	//turn off Analog Comparator
	ACSR |= (1 << ACD);

	//turning off other modules except timer 0 and 2
	PRR |= (1 <<  PRTWI) |  (1 <<   PRTIM1) | (1 <<  PRSPI)| (1 << PRUSART0)| (1 << PRADC);
	
	/*
	//turn off digital input buffers for analog channels
	DIDR1 |= (1 << AIN1D)  | (1 <<  AIN0D);
	*/

	//global interrupts on
	sei();

	//sleep mode with counter2 still active (Extended Standby)
	SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0);
}
