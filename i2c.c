#include "i2c.h"

int i2c_init() {
	I2C_PORT |= (1 << SDA_PIN | 1 << SCL_PIN);	
	TWCR = 0;
	TWSR = 0;
	TWBR = 0; //((F_CPU / SCL_CLOCK) - 16) / 2;
	_delay_ms(50);
	return 0;
}

int i2c_start(uint8_t address) {
	uint8_t timeout = 0;

	TWCR = (1 << TWINT | 1 << TWSTA | 1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {
		timeout++;
		if (timeout > I2C_TIMEOUT) {
			return ERR_I2C_TIMEOUT;
		}
	}
	if ((TWSR & 0xf8) != TW_START) {
		return ERR_I2C_START;
	}
	timeout = 0;
	TWDR = address;
	TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > I2C_TIMEOUT) {
			return ERR_I2C_TIMEOUT;
                }
        }
	if ((TWSR & 0xf8) != TW_MT_SLA_ACK) {
		return ERR_I2C_SLAVE;
	}

	return 0;
}

uint8_t i2c_read() {
	return 0;
}

int i2c_write(uint8_t data) {
	uint8_t timeout = 0;
        TWDR = data;
        TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > I2C_TIMEOUT) {
			signal_error(ERR_I2C_TIMEOUT+2);
			return ERR_I2C_TIMEOUT;
                }
        }
	//_delay_ms(10);
	return 0;
}

int i2c_stop() {
        TWCR = (1 << TWINT | 1 << TWSTO | 1 << TWEN);
	TWCR = 0;
	//_delay_ms(10);
}
