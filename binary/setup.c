#include "context.h"

const int HOURS_THRESHOLD = 24;
const int SECOND_MINUTES_THRESHOLD = 60;
//auto sleep after 15s
const int AWOKE_TIME_IN_SECONDS = 15;

void setupClock(Clock *clock) {
	clock->hours = 13;
	clock->minutes = 50;
	clock->seconds = 00;
	clock->awokeTimeCounterSeconds = 0;
}

void setupMikroController() {
	//I/O
	//set Data-Direction Output for LED
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
	DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD5) | (1 << DDD6);

	//Input for Buttons
	DDRD &= ~(BUTTON1);
	DDRD &= ~(BUTTON2);
	DDRD &= ~(BUTTON3);

	//pull up for Buttons on
	PORTD |= BUTTON1 | BUTTON2 | BUTTON3;

	//Energy saving
	//turn off Analog Comparator
	ACSR |= (1 << ACD);

	//turning off other modules except timer 0 and 2
	PRR |= (1 << PRTWI) |  (1 << PRTIM1) | (1 << PRSPI)| (1 << PRUSART0)| (1 << PRADC);
	
	//turn off digital input buffers for analog channels
	DIDR1 |= (1 << AIN1D)  | (1 <<  AIN0D);
	
	//Counter
	//Using asynchronous timer with low frequency crystal
	ASSR |= (1 << AS2);

	//Prescaler 128 (256x128 = 32768))
	TCCR2B |= (1 << CS22) | (1 << CS20);

	//Overflow-Counter 2 Interrupt on -> overflow every 1sec precisly
	TIMSK2 |= (1 << TOIE2);

	//counter0 with Prescaler 8
	TCCR0B |= (1 << CS21);
	
	//OutputcompareMatch Interrupt for counter 0 on
	TIMSK0 |= (1 << OCIE0A);
	
	//CTC Mode
	TCCR0A |= (1 << WGM01);
	
	//Adjust value between ca. 80-120 to flickering tolerance, higher more flickering, but more idle, less power consummation, all calculations with 8Mhz default clock of the ATMega48A
	OCR0A = 115;
	
	//global interrupts on
	sei();
}
