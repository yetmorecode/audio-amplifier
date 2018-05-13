//#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "i2c.h"
#include "ssd1306.h"

#define LED1 PB0
#define LED2 PD7
#define LED3 PD6

#define BUTTON1 PB1
#define BUTTON2 PB2

#define AUDIO_ADDRESS 0x58


#define ERROR_DELAY_MS 200
#define ERROR_DISPLAY_INIT 1

void led_print(uint8_t num) {
	if (num & 0x1) {
		PORTD |= (1 << LED3);
	} else {
		PORTD &= ~(1 << LED3);
	}
	if (num & 0x2) {
		PORTD |= (1 << LED2);
	} else {
		PORTD &= ~(1 << LED2);
	}
	if (num & 0x4) {
		PORTB |= (1 << LED1);
	} else {
		PORTB &= ~(1 << LED1);
	}
}

void led_init() {
	DDRB |= (1 << LED1);
	DDRD |= (1 << LED2);
	DDRD |= (1 << LED3);
	led_print(0);
}

typedef struct {
	uint8_t	mute_left;
	uint8_t mute_right;
	uint8_t gain;
	uint8_t agc;
} audio_status;

typedef struct {

} display_status;

void signal_error(uint8_t code) {
	while (code--) {
		PORTB |= (1 << LED1);
		_delay_ms(ERROR_DELAY_MS);
		PORTB &= ~(1 << LED1);
		_delay_ms(ERROR_DELAY_MS);
	}
	_delay_ms(10 * ERROR_DELAY_MS);
}


void pins_init() {
	DDRB &= ~(1 << BUTTON1);
	DDRB &= ~(1 << BUTTON2);

	PORTB |= (1 << BUTTON1);
	PORTB |= (1 << BUTTON2);
}


void handle_button1() {
	if (!(PINB & (1 << BUTTON1))) {
		PORTB |= (1 << LED1);
	} else {
		PORTB &= ~(1 << LED1);
	}
}


void handle_button2() {

}

void drive_led1() {

}

#define LED_INIT_DELAY 250
int main(void) {
	int i;
	pins_init();
	led_init();
	led_print(1+2+4);
	_delay_ms(LED_INIT_DELAY);
	led_print(4);
	_delay_ms(LED_INIT_DELAY);
	led_print(2);
	_delay_ms(LED_INIT_DELAY);
	led_print(1);
	_delay_ms(LED_INIT_DELAY);
	led_print(1+2+4);
	_delay_ms(LED_INIT_DELAY);
	led_print(0);

	i2c_init();
	ssd1306_init();
	ssd1306_clear();
	for (i=0; i < 5; i++) {
		ssd1306_write(i+1, i, "fix now :)");
	}

	while(1) {
		//handle_button1();
		//handle_button2();
		//ssd1306_write("hello");
		drive_led1();
	}
	return 0;
}

