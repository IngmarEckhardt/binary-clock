#ifndef CONTEXT_H_
#define CONTEXT_H_

#define MINUTES_LED		0b1000
#define HOURS_LED		0b10000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

extern const int HOURS_THRESHOLD;
extern const int SECOND_MINUTES_THRESHOLD;
//auto sleep after 12s
extern const int AWOKE_TIME_IN_SECONDS;

//bits of the state-byte in ClockState
enum ClockStates {
	SET_TIME			= 0b00000001,
	SET_HOUR			= 0b00000010,
	BUTTONS				= 0b00011100,
	BUTTON1				= 0b00000100,
	BUTTON2				= 0b00001000,
	BUTTON3				= 0b00010000,
	SECONDS				= 0b00100000,
	BUTTON_PRESSED		= 0b01000000,
	STANDBY				= 0b10000000,
};

typedef struct {
	volatile uint8_t state;
	volatile uint8_t idleCounter;
	volatile uint8_t seconds;
	volatile uint8_t minutes;
	volatile uint8_t hours;
	volatile uint8_t awokeTimeCounterSeconds;
	
} Clock;

void setupMikroController();
void setupClock(Clock *clock);
void readAllButtons(Clock *clock);
void turnOffLed(uint8_t portMask);
void showMinutesOrSeconds(uint8_t value);
void showHours(uint8_t value);
void handleDisplay(uint8_t idleCounter);
void handleSetTimeMode(Clock *clock);
void processUserInput(Clock *clock);

#endif /* CONTEXT_H_ */