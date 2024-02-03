#include "context.h"


void readButton(ButtonState *button) {
	button ->button_changeLog = (button->button_changeLog << 1) | !((PIND & (1 << button->pin)) >> button->pin);
}

void readAllButtons(ButtonState buttons[], uint8_t numButtons) {
	for (uint8_t i = 0; i < numButtons; ++i) {
		readButton(&buttons[i]);
	}
}

uint8_t isPressed(ButtonState *button) {
	uint8_t bitmask = 0b111;
	if ((button->button_changeLog&bitmask) == bitmask) {
		button->button_value = TRUE;
		} else if ((button->button_changeLog&bitmask) == 0) {
		button->button_value = FALSE;
	}
	return button->button_value;
}

uint8_t anyButtonPressed(ButtonState buttons[], uint8_t numButtons) {
	for (uint8_t i = 0; i < numButtons; ++i) {
		if (isPressed(&buttons[i])) {
			return TRUE; // At least one button is pressed
		}
	}
	return FALSE; // No buttons are pressed
}