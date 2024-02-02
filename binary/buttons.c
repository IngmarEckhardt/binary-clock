#include "context.h"

uint8_t checkWakeUpButtonInterruptFlag() {

	if (EIFR & (1 << INTF0)) {
		//reset the flag
		EIFR &= ~(1 << INTF0);
		return TRUE;
	}
	return FALSE;
}

void readAllButtons(ClockState *clock, ButtonStates *buttons) {
	uint8_t intermediateState = !((PIND & (1 << BUTTON1)) >> BUTTON1);
	if (intermediateState != buttons->button1){
		buttons ->button1_changed = TCNT2;
		buttons ->button1 = intermediateState;
	}
	intermediateState = !((PIND & (1 << BUTTON2)) >> BUTTON2);
	if (intermediateState != buttons->button2){
		buttons ->button2_changed = TCNT2;
		buttons ->button2 = intermediateState;
	}
	intermediateState = !((PIND & (1 << BUTTON3)) >> BUTTON3);
	if (intermediateState != buttons->button3){
		buttons ->button3_changed = TCNT2;
		buttons ->button3 = intermediateState;
	}
}

uint8_t buttonIsDebounced(uint8_t changeTimestamp) {
	uint8_t result;
	if (TCNT2 < changeTimestamp) {
		result = TCNT2 + 256 - changeTimestamp;
		} else {
		result = TCNT2 - changeTimestamp;
	}
	if (result>=4){
		return TRUE;
		} else {
		return FALSE;
	}
}

uint8_t allButtonAreDebounced(ButtonStates *buttons) {
	if (buttonIsDebounced(buttons->button1_changed)&&buttonIsDebounced(buttons->button2_changed)&&buttonIsDebounced(buttons->button3_changed)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

uint8_t anyButtonPressed(ButtonStates *buttons) {
	return (buttons ->button1 || buttons ->button2 || buttons ->button3) ? 0 : 1;
}