#include "context.h"

const int HOURS_THRESHOLD = 24;
const int SECOND_MINUTES_THRESHOLD = 60;
//auto sleep after 15s
const int AWOKE_TIME_IN_SECONDS = 15;

void setupClock(ClockState *clock) {
	clock->hours = 13;
	clock->minutes = 59;
	clock->seconds = 49;
	clock->idleCounter = 0;
	clock->awokeTimeCounterSeconds = 0;
}

void setupMikroController() {
	//set Data-Direction Output for LED
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD5) | (1 << DDD6);

	//Input for Buttons
	DDRD &= ~(BUTTON1);
	DDRD &= ~(BUTTON2);
	DDRD &= ~(BUTTON3);

	//pull up for Button 1 always on
	PORTD |= BUTTON1 | BUTTON2 | BUTTON3;

	//turn off Analog Comparator
	ACSR |= (1 << ACD);

	//turning off other modules except timer 0 and 2
	PRR |= (1 << PRTWI) |  (1 << PRTIM1) | (1 << PRSPI)| (1 << PRUSART0)| (1 << PRADC);
	
	//turn off digital input buffers for analog channels
	DIDR1 |= (1 << AIN1D)  | (1 <<  AIN0D);
	
	//Using asynchronous timer with low frequency crystal
	ASSR |= (1 << AS2);

	//pre scaler 128 (256x128 = 32768))
	TCCR2B |= (1 << CS22) | (1 << CS20);

	//Overflow-Counter Interrupt on
	TIMSK2 |= (1 << TOIE2);

	//counter0 on without pre scaling
	TCCR0B |= (1 << CS21);
	//Timer-Overflow Interrupt on
	TIMSK0 |= (1 << OCIE0A);
	//CTC Mode
	TCCR0A |= (1 << WGM01);
	
	//Adjust value to flickering tolerance, higher more flickering, but more idle, less power consumation
	OCR0A = 110;
	
	//global interrupts on
	sei();
}
