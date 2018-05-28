#include "timing.h"

volatile uint8_t t1 = 0;
volatile uint8_t t2 = 0;
volatile uint8_t t3 = 0;

ISR(TIMER0_OVF_vect) {
	t1++;
	if (t1 == 255) {
		t1 = 0;
		t2++;
	}
	if (t2 == 255) {
		t2 = 0;
		t3++;
	}
	if (t3 == 255) {
		t3 = 0;
	}
}

void timing_init() {
	PRR &= ~(1 << PRTIM0);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TCNT0 = 0;
	TIMSK0 |= (1 << TOIE0);
	sei();
}

uint8_t timing_counter (int number) {
	switch (number) {
		case 3:
			return t3;
		case 2:
			return t2;
		case 1:
			return t1;
		default:
			return 0;
	}
}
