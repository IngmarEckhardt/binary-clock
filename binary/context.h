#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
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

extern const uint8_t MODE_BITMASK;
extern const uint8_t SECONDS_MODE_BITMASK;
extern const uint8_t SET_HOUR_BITMASK;
extern const uint8_t SET_TIME_BITMASK;

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
	DIMMED_SET_TIME_HOURS_MODE_SHOW_SECONDS = 0b1111,
	STANDBY = 0b10000000,
	BUTTON_PRESSED = 0b01000000
};

typedef struct {
	uint8_t pin;
	volatile uint8_t button_value;
	volatile uint8_t button_changeLog;
} ButtonState;

typedef struct {
	volatile uint8_t state;
	
	volatile uint8_t idleCounter;
	volatile uint8_t seconds;
	volatile uint8_t minutes;
	volatile uint8_t hours;
	volatile uint8_t awokeTimeCounterSeconds;

} ClockState;


void setupMikroController();
void setupClock(ClockState *clock, ButtonState buttons[]);
void readButton(ButtonState *button);
void readAllButtons(ButtonState buttons[], uint8_t numButtons);
uint8_t isPressed(ButtonState *button);
uint8_t anyButtonPressed(ButtonState buttons[], uint8_t numButtons);



void turnOffLed(uint8_t portMask);
void showMinutesOrSeconds(uint8_t value);
void showHours(uint8_t value);
void handleDisplay(uint8_t idleCounter);
void handleSetTimeMode(ClockState *clock, ButtonState *buttonStates);
void processUserInput(ClockState *clock, ButtonState *buttons);

#endif /* CONTEXT_H_ */