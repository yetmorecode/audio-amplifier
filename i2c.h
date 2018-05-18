#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define SDA_PIN	PC4
#define SCL_PIN	PC5
#define I2C_PORT PORTC
#define SCL_CLOCK 400000UL
#define I2C_TIMEOUT 1000

#define ERR_I2C_START 2
#define ERR_I2C_SLAVE 3
#define ERR_I2C_TIMEOUT 5

#define I2C_WRITE 0
#define I2C_READ 1

int	i2c_init();
int	i2c_start(uint8_t address, uint8_t direction);
int	i2c_write(uint8_t date);
int	i2c_read(uint8_t *data, uint8_t ack);
int	i2c_stop();

#endif
