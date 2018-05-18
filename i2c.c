#include "i2c.h"

int i2c_init() {
	I2C_PORT |= (1 << SDA_PIN | 1 << SCL_PIN);	
	TWCR = 0;
	TWSR = 0;
	TWBR = 0; //((F_CPU / SCL_CLOCK) - 16) / 2;
	//_delay_ms(10);
	return 0;
}

int i2c_start(uint8_t address, uint8_t direction) {
	uint8_t timeout = 0;
	uint8_t status;

	TWCR = (1 << TWINT | 1 << TWSTA | 1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {
		timeout++;
		if (timeout > I2C_TIMEOUT) {
			return ERR_I2C_TIMEOUT;
		}
	}
	switch ((status = TW_STATUS)) {
		case TW_START:
		case TW_REP_START:
			break;
		default:
			return ERR_I2C_START;
	}
	timeout = 0;
	TWDR = (address << 1) | direction;
	TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > I2C_TIMEOUT) {
			return ERR_I2C_TIMEOUT;
                }
        }
	if (direction == TW_WRITE) {
		if ((TWSR & 0xf8) != TW_MT_SLA_ACK) {
			return ERR_I2C_SLAVE;
		}
	} else {
		/*if ((TWSR & 0xf8) != TW_MR_SLA_ACK) {
			return ERR_I2C_SLAVE;
		}*/
	}

	return 0;
}

int i2c_read(uint8_t *data, uint8_t ack) {
	uint8_t timeout = 0;
	uint8_t status;

	TWCR = _BV(TWINT) | _BV(TWEN) | (ack ? _BV(TWEA) : 0);
	while ((TWCR & _BV(TWINT)) == 0) {
                timeout++;
                if (timeout > I2C_TIMEOUT) {
			return ERR_I2C_TIMEOUT;
                }
	}
	switch ((status = TW_STATUS)) {
		case TW_MR_DATA_NACK:
		case TW_MR_DATA_ACK:
			*data = TWDR;
			break;
		default:
			*data = 0xff;
			return 10;
	}
	return 0;
}

int i2c_write(uint8_t data) {
	uint8_t timeout = 0;
        TWDR = data;
        TWCR = (1 << TWINT | 1 << TWEN);
        while (!(TWCR & (1 << TWINT))) {
                timeout++;
                if (timeout > I2C_TIMEOUT) {
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
