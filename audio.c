//#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "i2c.h"
#include "ssd1306.h"
#include "tpa2016.h"

#define LED1 PB0
#define LED2 PD7
#define LED3 PD6

#define BUTTON1 PB1
#define BUTTON2 PB2

#define ERROR_DELAY_MS 200
#define ERROR_DISPLAY_INIT 1

#define LED_INIT_DELAY 250

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

	/*
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
	*/
	led_print(0);

}

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

tpa2016_control amp = { 0 };


int updateDisplay = 0;

void update_display() {
	if (!updateDisplay) {
		return;
	}

	tpa2016_get(&amp);
	ssd1306_printf(0, 0, "home amplifier");
	ssd1306_printf(1, 0, "version 1.0");

	if (amp.fault_left || amp.fault_right || amp.thermal_shutdown) {
		ssd1306_printf(2, 1, 
			"%s%s%s",
			amp.fault_left ? "FL " : "",
			amp.fault_right ? "FR " : "",
			amp.thermal_shutdown ? "TS " : ""
		);
	}
	ssd1306_printf(3, 1, 
		"%s %i / %i dB %s", 
		amp.speaker_enable_left ? "L" : " ", 
		(amp.agc_fixed_gain > 30 ? (-36 + (amp.agc_fixed_gain & 0b11111)) : (amp.agc_fixed_gain)),
		18 + amp.max_gain,
		amp.speaker_enable_right ? "R" : " "
	);
	char *ng;
	switch (amp.noise_gate_threshold) {
		case 1:
			ng = "1mV";
			break;
		case 2:
			ng = "4mV";
			break;
		case 3:
			ng = "10mV";
			break;
		case 4:
		default:
			ng = "20mV";
			break;
	}

	ssd1306_printf(4, 1,
		"%i:1 %s%s",
		(1 << amp.compression_ratio),
		amp.noise_gate_enable ? "NG " : "",
		amp.noise_gate_enable ? ng : ""
	);

	ssd1306_printf(7, 0, 
		"%.2x%.2x%.2x%.2x%.2x%.2x%.2x", 
		amp.raw[0], amp.raw[1], amp.raw[2], amp.raw[3], 
		amp.raw[4], amp.raw[5], amp.raw[6]
	);
	updateDisplay = 0;
}

int main(void) {
	int status;
	int i, count;
	pins_init();
	led_init();

	i2c_init();
	ssd1306_init();
	ssd1306_clear();
	ssd1306_printf(0, 0, "home amplifier");
	ssd1306_printf(1, 0, "version 1.0");
	// give tpa2016 some bootup time
	_delay_ms(50);
	tpa2016_init(&amp);
	amp.agc_fixed_gain = 12;
	amp.max_gain = 0;
	amp.compression_ratio = 0;
	tpa2016_set(&amp);
	updateDisplay = 1;
	update_display();


	count = 0;	
	while(1) {
		//handle_button1();
		//handle_button2();
		//ssd1306_write("hello");
		drive_led1();
		count++;

		if (count % 50 == 0) {
			//amp.speaker_enable_left = !amp.speaker_enable_left;
			//amp.speaker_enable_right = !amp.speaker_enable_left;
			//tpa2016_set(&amp);
			updateDisplay = true;
		}

		update_display();
		_delay_ms(100);
	}
	return 0;
}

