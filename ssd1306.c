#include "ssd1306.h"

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

void ssd1306_gotoxy(uint8_t row, uint8_t column) {
	i2c_start(SSD1306_ADDRESS);
	i2c_write(0x00);
	i2c_write(SET_PAGE_START | row);
	i2c_write(SET_COLUMN_ADDRESS);
	i2c_write(column * 8);
	i2c_write(SET_DISPLAY_START_LINE | 0x3f);
	i2c_stop();
}

void ssd1306_clear() {
	uint16_t i;
	ssd1306_gotoxy(0, 0);
	i2c_start(SSD1306_ADDRESS);
	i2c_write(0x40);
	for (i=0; i < 128<<4; i++) {
		i2c_write(0);
	}
	i2c_stop();
}

void ssd1306_init() {
	uint8_t tmp;
	_delay_ms(500);
	if (i2c_start(SSD1306_ADDRESS)) {
		return;
	}
	if (i2c_write(0x00)) {
		return;
	}
	for (tmp = 0; tmp < sizeof(ssd1306_init_cmd); tmp++) {	
		if (i2c_write(pgm_read_byte(&ssd1306_init_cmd[tmp]))) {
			return;
		}
	}
	ssd1306_clear();
}

void ssd1306_write_char(uint8_t c) {
	uint8_t i;
	i2c_start(SSD1306_ADDRESS);
	i2c_write(0x40);
	for (i=0; i < 8; i++) {
		i2c_write(pgm_read_byte(&font[(unsigned char)c][i]));
	}
	i2c_stop();
}

void ssd1306_write(uint8_t row, uint8_t column, char *str) {
	uint16_t i = 0;
	while(*str) {
		ssd1306_gotoxy(row, column++);
		ssd1306_write_char(*str++);
		i++;
	}
}
