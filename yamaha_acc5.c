#include "yamaha_acc5.h"

int cc5_init() {
	CC5_LED_JAZZ_DDR |= (1 << CC5_LED_JAZZ_PIN);
	CC5_LED_ROCK_DDR |= (1 << CC5_LED_ROCK_PIN);
	CC5_LED_HALL_DDR |= (1 << CC5_LED_HALL_PIN);

	cc5_disable_led(CC5_LED_JAZZ);
	cc5_disable_led(CC5_LED_ROCK);
	cc5_disable_led(CC5_LED_HALL);

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
	}

	return 0;
}

bool cc5_is_button_pressed(int button) {

	return false;
}

int cc5_led_print(int num) {
	if (num & 0x1) {
		cc5_enable_led(CC5_LED_ROCK);
	} else {
		cc5_disable_led(CC5_LED_ROCK);
	}
	if (num & 0x2) {
		cc5_enable_led(CC5_LED_JAZZ);
	} else {
		cc5_disable_led(CC5_LED_JAZZ);
	}
	if (num & 0x4) {
		cc5_enable_led(CC5_LED_HALL);
	} else {
		cc5_disable_led(CC5_LED_HALL);
	}
	return 0;
}
