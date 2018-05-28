#ifndef __TIMING_H
#define __TIMING_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Timer0 overflows into t1. t1 overflows into t2, ...
 */
extern volatile uint8_t t1;
extern volatile uint8_t t2;
extern volatile uint8_t t3;

void timing_init();
uint8_t timing_counter(int number);

#endif
