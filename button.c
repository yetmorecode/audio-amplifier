#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#define LED1 PB0
#define BUTTON1 PB1
#define BUTTON2 PB2

#define DISPLAY_ADDRESS 0x3c
#define AUDIO_ADDRESS 0x58

#define SET_CONTRAST_CONTROL		0x81
#define SET_DISPLAY_ON			0xAF
#define SET_DISPLAY_OFF			0xAE
#define SET_DISPLAY_NORMAL		0xA6
#define SET_DISPLAY_INVERSE		0xA7
#define ENTIRE_DISPLAY_ON		0xA4
#define SET_MEMORY_ADDR_MODE		0x20
#define HORIZONTAL_ADDRESSING_MODE	0x00
#define VERTICAL_ADDRESSING_MODE	0x01
#define PAGE_ADDRESSING_MODE		0x02
#define SET_PAGE_START			0xB0	//~0xB7
#define SET_COLUMN_ADDRESS		0x21
#define SET_PAGE_ADDRESS		0x22
#define SET_COM_OUTPUT_NORMAL		0xC0
#define SET_COM_OUTPUT_REMAPPED		0xC8
#define SET_DISPLAY_OFFSET		0xD3
#define SET_DISPLAY_CLOCK		0xD5
#define SET_PRECHARGE_PERIOD		0xD9
#define SET_COM_PINS			0xDA
#define SET_VCOMH_DESELECT		0xDB	//0x00,0x20,0x30
#define	SET_LOWER_COLUMN		0x00
#define	SET_HIGHER_COLUMN		0x10
#define	SET_DISPLAY_START_LINE		0x40
#define SET_SEGMENT_REMAP_O1		0xA0	
#define SET_SEGMENT_REMAP_O2		0xA1
#define SET_MULTIPLEX_RATIO		0xA8
#define COMMAND_NO_OPERATION		0xE3
#define CHARGE_BUMB_SETTING		0x8D

const uint8_t ssd1306_init_cmd [] PROGMEM = {
	SET_DISPLAY_OFF,
	COMMAND_NO_OPERATION,
	SET_MEMORY_ADDR_MODE, HORIZONTAL_ADDRESSING_MODE,
	SET_PAGE_START,
	SET_COM_OUTPUT_REMAPPED, 
	SET_LOWER_COLUMN,
	SET_HIGHER_COLUMN,
	SET_DISPLAY_START_LINE,			
	ENTIRE_DISPLAY_ON, 
	SET_SEGMENT_REMAP_O2,
	SET_DISPLAY_NORMAL,	
	SET_CONTRAST_CONTROL,		0xFF,	
	SET_MULTIPLEX_RATIO,		0x3F,	
	SET_DISPLAY_OFFSET,		0x00, 
	SET_DISPLAY_CLOCK,		0xF0,
	SET_PRECHARGE_PERIOD,		0x22, 
	SET_COM_PINS,			0x12, 
	SET_VCOMH_DESELECT,		0x20, 
	CHARGE_BUMB_SETTING,		0x14, 
	SET_DISPLAY_ON 
};

typedef struct {
	uint8_t	mute_left;
	uint8_t mute_right;
	uint8_t gain;
	uint8_t agc;
} audio_status;

typedef struct {

} display_status;

void display_update() {

}

#define SDA_PIN	PC4
#define SCL_PIN	PC5
#define I2C_PORT PORTC
#define SCL_CLOCK 400000UL

void i2c_init() {
	I2C_PORT |= (1 << SDA_PIN | 1 << SCL_PIN);	
	TWCR = 0;
	TWSR = 0;
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
	_delay_ms(50);
}

int i2c_start(uint8_t address) {
	uint8_t timeout = 0;

	TWCR = (1 << TWINT | 1 << TWSTA | 1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {
		timeout++;
		if (timeout > 100) {
			return 1;
		}
	}
	timeout = 0;
	TWDR = address;
	TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > 100) {
                        return 1;
                }
        }

	return 0;
}

int i2c_byte(uint8_t data) {
	uint8_t timeout = 0;
        TWDR = data;
        TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > 100) {
                        return 1;
                }
        }
	return 0;
}

int i2c_stop() {
        TWCR = (1 << TWINT | 1 << TWSTO | 1 << TWEN);
	TWCR = 0;
}

void display_init() {
	uint8_t tmp;
	i2c_start(DISPLAY_ADDRESS);
	i2c_byte(0x00);
	for (tmp = 0; tmp < sizeof(ssd1306_init_cmd); tmp++) {	
		i2c_byte(pgm_read_byte(&ssd1306_init_cmd[tmp]));
	}
	
}

void init() {
	DDRB |= (1 << LED1);
	DDRB &= ~(1 << BUTTON1);
	DDRB &= ~(1 << BUTTON2);

	PORTB |= (1 << LED1);
	PORTB |= (1 << BUTTON1);
	PORTB |= (1 << BUTTON2);

	i2c_init();
	display_init();
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

int main(void) {

	init();

	while(1) {
		handle_button1();
		handle_button2();
		drive_led1();
		display_update();
	}
	return 0;
}

