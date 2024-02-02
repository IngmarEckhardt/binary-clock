#ifndef CONTEXT_H_
#define CONTEXT_H_
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

//define PINs
#define BUTTON1 PD2
#define BUTTON2 PD3
#define BUTTON3 PD4
#define MINUTES_LED (1 << PC3)
#define HOURS_LED (1 << PC4)
//make it more readable
#define TRUE 1
#define FALSE 0

extern const int HOURS_THRESHOLD;
extern const int SECOND_MINUTES_THRESHOLD;
//auto sleep after 15s
extern const int AWOKE_TIME_IN_SECONDS;
//debouncing with 10ms
extern const int DEBOUNCE_DELAY;
//doubled value for 3 blinks
extern const int MAX_BLINK_AMOUNT;

enum ClockStates {
	NOT_DIMMED = 0b00,
	NOT_DIMMED_SET_TIME = 0b10,
	NOT_DIMMED_SET_TIME_HOURS_MODE = 0b110,
	NOT_DIMMED_SHOW_SECONDS = 0b1000,
	NOT_DIMMED_SET_TIME_SHOW_SECONDS = 0b1010,
	NOT_DIMMED_SET_TIME_HOURS_MODE_SHOW_SECONDS = 0b1110,
	DIMMED = 0b01,
	DIMMED_SET_TIME = 0b11,
	DIMMED_SET_TIME_HOURS_MODE = 0b111,
	DIMMED_SHOW_SECONDS = 0b1001,
	DIMMED_SET_TIME_SHOW_SECONDS = 0b1011,
	DIMMED_SET_TIME_HOURS_MODE_SHOW_SECONDS = 0b1111
};

typedef struct {
	uint8_t button1;
	uint8_t button1_changed;
	uint8_t button2;
	uint8_t button2_changed;
	uint8_t button3;
	uint8_t button3_changed;
} ButtonStates;

typedef struct {
	uint8_t blinkCounter;
	uint8_t ledsOn;
	uint8_t buttonPressed;
	uint8_t state;
	
	volatile uint8_t seconds;
	volatile uint8_t minutes;
	volatile uint8_t hours;
	volatile uint8_t awokeTimeCounterSeconds;
	volatile uint8_t wakeUpFlag;
} ClockState;


void setupMikroController();
void setupClock(ClockState *clock);
uint8_t allButtonAreDebounced(ButtonStates *buttons);
uint8_t anyButtonPressed(ButtonStates *buttons);
void readAllButtons(ClockState *clock, ButtonStates *buttons);
uint8_t checkWakeUpButtonInterruptFlag() ;

void turnOffLed(ClockState *clock, uint8_t portMask);
void showMinutesOrSeconds(uint8_t value);
void blinkWithLed(ClockState *clock, uint8_t portMask);
void showHours(uint8_t value);
void handleUndimmedState(ClockState *clock);

void handleSetTimeMode(ClockState *clock, ButtonStates *buttonStates);

void processUserInput(ClockState *clock, ButtonStates *buttons);

#endif /* CONTEXT_H_ */