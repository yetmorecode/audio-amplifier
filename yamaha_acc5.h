/**
 * API to control the frontplate of the yamaha apmlifier A-CC5
 */
#ifndef __YAMAHA_ACC5_H
#define __YAMAHA_ACC5_H

#include <avr/io.h>
#include <stdbool.h>
#include "timing.h"

#define CC5_BUTTON_POWER 1
#define CC5_BUTTON_POWER_READ PINB
#define CC5_BUTTON_POWER_WRITE PORTB
#define CC5_BUTTON_POWER_PIN PB2
#define CC5_BUTTON_POWER_DDR DDRB
#define CC5_BUTTON_MODE 2
#define CC5_BUTTON_MODE_READ PINB
#define CC5_BUTTON_MODE_WRITE PORTB
#define CC5_BUTTON_MODE_PIN PB1
#define CC5_BUTTON_MODE_DDR DDRB

#define CC5_LED_POWER 1
#define CC5_LED_HALL 2
#define CC5_LED_JAZZ 3
#define CC5_LED_ROCK 4
#define CC5_LED_TAPE 5
#define CC5_LED_PHONO 6
#define CC5_LED_CD 7
#define CC5_LED_TUNER 8
#define CC5_LED_AUX 9

#define CC5_LED_HALL_PIN PD7
#define CC5_LED_JAZZ_PIN PB0
#define CC5_LED_ROCK_PIN PD6
#define CC5_LED_CD_PIN PB7
#define CC5_LED_TUNER_PIN PB6

#define CC5_LED_HALL_PORT PORTD
#define CC5_LED_JAZZ_PORT PORTB
#define CC5_LED_ROCK_PORT PORTD
#define CC5_LED_CD_PORT PORTB
#define CC5_LED_TUNER_PORT PORTB

#define CC5_LED_HALL_DDR DDRD
#define CC5_LED_JAZZ_DDR DDRB
#define CC5_LED_ROCK_DDR DDRD
#define CC5_LED_CD_DDR DDRB
#define CC5_LED_TUNER_DDR DDRB

#define CC5_MC14052B_A_PIN PD3
#define CC5_MC14052B_B_PIN PD4
#define CC5_MC14052B_A_PORT PORTD
#define CC5_MC14052B_B_PORT PORTD
#define CC5_MC14052B_A_DDR DDRD
#define CC5_MC14052B_B_DDR DDRD

int 	cc5_init();
int 	cc5_enable_led(int led);
int 	cc5_disable_led(int led);
int	cc5_led_print(int num, int input);
bool	cc5_is_button_pressed(int button);
bool	cc5_is_button_longpressed(int button);

#endif
