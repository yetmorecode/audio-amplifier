#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdint.h>
#include "i2c.h"
#include "ssd1306.h"
#include "tpa2016.h"
#include "yamaha_acc5.h"
#include <avr/power.h>

#define soft_reset()        \
do {                        \
    wdt_enable(WDTO_15MS);  \
    for(;;) { }             \
} while(0)


tpa2016_control amp = { 0 };
int updateDisplay = 0;

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
void wdt_init() {
    MCUSR = 0;
    wdt_disable();
}

void update_display();

int init() {
	int i;

	// pullup reset pin
	DDRC |= (1 << PC6);
	PORTC |= (1 << PC6);

	// init ACC5 frontplate
	cc5_init();
	for (i=1; i < 5; i++) {
		cc5_led_print(7, i);
		_delay_ms(200);
	}
	for (i=5; i > 0; i--) {
		cc5_led_print(7, i);
		_delay_ms(200);
	}
/*
	cc5_led_print(1, 0);
	_delay_ms(200);
	cc5_led_print(2, 0);
	_delay_ms(200);
	cc5_led_print(4, 0);
	_delay_ms(200);
	cc5_led_print(2, 0);
	_delay_ms(200);
	cc5_led_print(1, 0);
	_delay_ms(200);
*/
	cc5_led_print(0, 0);

	// start I2C
	i2c_init();

	// start display
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

	return 0;
}

int balance = 0;
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
		//(amp.agc_fixed_gain > 30 ? (-36 + (amp.agc_fixed_gain & 0b11111)) : (amp.agc_fixed_gain)),
		balance,
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

static int reset = 0;
void handle_reset() {
	if (cc5_is_button_pressed(CC5_BUTTON_POWER)) {
		cc5_enable_led(CC5_LED_CD);
		reset++;
	} else {
		cc5_disable_led(CC5_LED_CD);
		reset = 0;
	}
	if (reset > 10) {
		soft_reset();
	}
}

void handle_mode() {
	int a;
	if (cc5_is_button_pressed(CC5_BUTTON_MODE)) {
		a = 7;
	} else {
		a = 0;
	}
	cc5_led_print(a, -1);
}

void handle_balance() {
	int timeout = 0;
	ADMUX = 0b01100000;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) {
		if (timeout++ > 100) {
			balance = 7;
			return;
		}
	}
	balance = 0xff - ADCH;
	ADCSRA &= ~(1 << ADEN);
}

int main(void) {
	int count = 0;
	static int last = 0;

	init();

	// enable adc
	PRR &= ~(1 << PRADC);

	while(1) {
		handle_reset();
		handle_mode();

		if (count % 50 == 0) {
			//amp.speaker_enable_left = !amp.speaker_enable_left;
			//amp.speaker_enable_right = !amp.speaker_enable_left;
			//tpa2016_set(&amp);
			updateDisplay = true;
		}

		handle_balance();
		if (balance != last) {
			last = balance;
			updateDisplay = true;
		}

		update_display();
		count++;
		_delay_ms(100);
	}
	return 0;
}

