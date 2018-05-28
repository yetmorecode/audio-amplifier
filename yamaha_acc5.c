#include "yamaha_acc5.h"

/*
 * Some LEDs are controlled through a MC14052B on the ACC5 frontplate
 */
static void set_mc14052b(bool a, bool b) {
	if (a) {
		CC5_MC14052B_A_PORT |= (1 << CC5_MC14052B_A_PIN);
	} else {
		CC5_MC14052B_A_PORT &= ~(1 << CC5_MC14052B_A_PIN);
	}
	if (b) {
		CC5_MC14052B_B_PORT |= (1 << CC5_MC14052B_B_PIN);
	} else {
		CC5_MC14052B_B_PORT &= ~(1 << CC5_MC14052B_B_PIN);
	}
}

int cc5_init() {
	// Setup directly addressed LEDs
	CC5_LED_JAZZ_DDR |= (1 << CC5_LED_JAZZ_PIN);
	CC5_LED_ROCK_DDR |= (1 << CC5_LED_ROCK_PIN);
	CC5_LED_HALL_DDR |= (1 << CC5_LED_HALL_PIN);
	CC5_LED_CD_DDR |= (1 << CC5_LED_CD_PIN);
	CC5_LED_TUNER_DDR |= (1 << CC5_LED_TUNER_PIN);
	
	// Setup LED multiplexer
	CC5_MC14052B_A_DDR |= (1 << CC5_MC14052B_A_PIN);
	CC5_MC14052B_B_DDR |= (1 << CC5_MC14052B_B_PIN);

	// Disable all LEDs initially
	cc5_disable_led(CC5_LED_JAZZ);
	cc5_disable_led(CC5_LED_ROCK);
	cc5_disable_led(CC5_LED_HALL);
	cc5_disable_led(CC5_LED_CD);
	cc5_disable_led(CC5_LED_TUNER);
	set_mc14052b(true, true);

	// Setup buttons with internal pullups
	CC5_BUTTON_MODE_DDR &= ~(1 << CC5_BUTTON_MODE_PIN);
	CC5_BUTTON_MODE_WRITE |= (1 << CC5_BUTTON_MODE_PIN);
	CC5_BUTTON_POWER_DDR &= ~(1 << CC5_BUTTON_POWER_PIN);
	CC5_BUTTON_POWER_WRITE |= (1 << CC5_BUTTON_POWER_PIN);

	return 0;
}

int cc5_enable_led(int led) {
	switch (led) {
		case CC5_LED_JAZZ:
			CC5_LED_JAZZ_PORT &= ~(1 << CC5_LED_JAZZ_PIN);
			break;
		case CC5_LED_ROCK:
			CC5_LED_ROCK_PORT &= ~(1 << CC5_LED_ROCK_PIN);
			break;
		case CC5_LED_HALL:
			CC5_LED_HALL_PORT &= ~(1 << CC5_LED_HALL_PIN);
			break;
		case CC5_LED_CD:
			set_mc14052b(true, true);
			cc5_disable_led(CC5_LED_TUNER);
			CC5_LED_CD_PORT |= (1 << CC5_LED_CD_PIN);
			break;
		case CC5_LED_TUNER:
			set_mc14052b(true, true);
			cc5_disable_led(CC5_LED_CD);
			CC5_LED_TUNER_PORT &= ~(1 << CC5_LED_TUNER_PIN);
			break;
		case CC5_LED_TAPE:
			cc5_disable_led(CC5_LED_CD);
			cc5_disable_led(CC5_LED_TUNER);
			set_mc14052b(false, true);
			break;
		case CC5_LED_AUX:
			cc5_disable_led(CC5_LED_CD);
			cc5_disable_led(CC5_LED_TUNER);
			set_mc14052b(false, false);
			break;
		case CC5_LED_PHONO:
			cc5_disable_led(CC5_LED_CD);
			cc5_disable_led(CC5_LED_TUNER);
			set_mc14052b(true, false);
			break;
	}

	return 0;
}

int cc5_disable_led(int led) {
	switch (led) {
		case CC5_LED_JAZZ:
			CC5_LED_JAZZ_PORT |= (1 << CC5_LED_JAZZ_PIN);
			break;
		case CC5_LED_ROCK:
			CC5_LED_ROCK_PORT |= (1 << CC5_LED_ROCK_PIN);
			break;
		case CC5_LED_HALL:
			CC5_LED_HALL_PORT |= (1 << CC5_LED_HALL_PIN);
			break;
		case CC5_LED_CD:
			CC5_LED_CD_PORT &= ~(1 << CC5_LED_CD_PIN);
			break;
		case CC5_LED_TUNER:
			CC5_LED_TUNER_PORT |= (1 << CC5_LED_TUNER_PIN);
			break;
		case CC5_LED_AUX:
		case CC5_LED_TAPE:
		case CC5_LED_PHONO:
			cc5_disable_led(CC5_LED_CD);
			cc5_disable_led(CC5_LED_TUNER);
			set_mc14052b(true, true);
			break;
	}

	return 0;
}

bool cc5_is_button_pressed(int button) {
	switch (button) {
		case CC5_BUTTON_MODE:
			return !(CC5_BUTTON_MODE_READ & (1 << CC5_BUTTON_MODE_PIN));
		case CC5_BUTTON_POWER:
			return !(CC5_BUTTON_POWER_READ & (1 << CC5_BUTTON_POWER_PIN));
	}

	return false;
}

#define CC5_LONGPRESS_DELAY 10
bool cc5_is_button_longpressed(int button) {
	static uint8_t b1 = 0, b2 = 0;
	uint8_t t = timing_counter(1);
	switch (button) {
		case CC5_BUTTON_MODE:
			if (!(CC5_BUTTON_MODE_READ & (1 << CC5_BUTTON_MODE_PIN))) {
				if (b1 == 0) {
					b1 = t;
				}
				if ((uint8_t)(t - b1) > CC5_LONGPRESS_DELAY) {
					return true;
				}
			} else {
				b1 = t;
			}
		case CC5_BUTTON_POWER:
			if (!(CC5_BUTTON_POWER_READ & (1 << CC5_BUTTON_POWER_PIN))) {
				if (b2 == 0) {
					b2 = t;
				}
				if ((uint8_t)(t - b2) > CC5_LONGPRESS_DELAY) {
					return true;
				}
			} else {
				b2 = t;
			}
	}

	return false;
}

int cc5_led_print(int mode, int input) {
	int mask = 1;
	if (mode & mask) {
		cc5_enable_led(CC5_LED_ROCK);
	} else {
		cc5_disable_led(CC5_LED_ROCK);
	}
	mask <<= 1;
	if (mode & mask) {
		cc5_enable_led(CC5_LED_JAZZ);
	} else {
		cc5_disable_led(CC5_LED_JAZZ);
	}
	mask <<= 1;
	if (mode & mask) {
		cc5_enable_led(CC5_LED_HALL);
	} else {
		cc5_disable_led(CC5_LED_HALL);
	}

	switch (input) {
		case 5:
			cc5_enable_led(CC5_LED_AUX);
			break;
		case 4:
			cc5_enable_led(CC5_LED_TUNER);
			break;
		case 3:
			cc5_enable_led(CC5_LED_CD);
			break;
		case 2:
			cc5_enable_led(CC5_LED_PHONO);
			break;
		case 1:
			cc5_enable_led(CC5_LED_TAPE);
			break;
		case 0:
			cc5_disable_led(CC5_LED_AUX);
			break;
		default:
			break;
	}
	return 0;
}
