#ifndef __SSD1306_H
#define __SSD1306_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <stdarg.h>
#include "i2c.h"
#include "font.h"
#include <stdio.h>

#define SSD1306_ADDRESS 0x3c

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

void ssd1306_init();
void ssd1306_clear();
void ssd1306_gotoxy(uint8_t row, uint8_t column);
void ssd1306_write_char(uint8_t c);
void ssd1306_write_number(uint8_t row, uint8_t column, uint8_t number);
void ssd1306_write(uint8_t row, uint8_t column, char *str);
void ssd1306_printf(uint8_t row, uint8_t column, char *fmt, ...);

#endif
