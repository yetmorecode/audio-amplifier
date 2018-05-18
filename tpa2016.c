#include "tpa2016.h"

#define TPA_BOOL(x) ((x) == 0 ? 0 : 1)

int tpa2016_init(tpa2016_control *dev) {
	tpa2016_get(dev);
	return 0;
}

int tpa2016_get(tpa2016_control *dev) {
	uint8_t data;
	int status;
	int i;

	status = i2c_start(TPA2016_ADDRESS, TW_WRITE);
	if (status != 0) {
		i2c_stop();
		return status;
	}
	status = i2c_write(1);
	if (status != 0) {
		i2c_stop();
		return status;
	}
	status = i2c_start(TPA2016_ADDRESS, TW_READ);
	if (status != 0) {
		i2c_stop();
		return status;
	}
	for (i = 0; i < 7; i++) {
		status = i2c_read(&data, (i==6 ? 0 : 1));
		if (status != 0) {
			i2c_stop();
			return status;
		}
		dev->raw[i] = data;
	}
	i2c_stop();

	dev->speaker_enable_right = TPA_BOOL(dev->raw[0] & (1 << 7));
	dev->speaker_enable_left = TPA_BOOL(dev->raw[0] & (1 << 6));
	dev->software_shutdown = TPA_BOOL(dev->raw[0] & (1 << 5));
	dev->fault_right = TPA_BOOL(dev->raw[0] & (1 << 4));
	dev->fault_left = TPA_BOOL(dev->raw[0] & (1 << 3));
	dev->thermal_shutdown = TPA_BOOL(dev->raw[0] & (1 << 2));
	dev->noise_gate_enable = TPA_BOOL(dev->raw[0] & (1 << 0));
	
	dev->agc_attack_time = dev->raw[1] & 0b111111;
	dev->agc_release_time = dev->raw[2] & 0b111111;
	dev->agc_hold_time = dev->raw[3] & 0b111111;
	dev->agc_fixed_gain = dev->raw[4] & 0b111111;

	dev->output_limiter_disable = TPA_BOOL(dev->raw[5] & (7 << 1));
	dev->noise_gate_threshold = (dev->raw[5] & 0b1100000) >> 5;
	dev->output_limiter_level = dev->raw[5] & 0b11111;

	dev->max_gain = (dev->raw[6] & 0b11110000) >> 4;
	dev->compression_ratio = dev->raw[6] & 0b11;
	
	return 0;
}

int tpa2016_set(tpa2016_control *dev) {
	int status, i;

	dev->raw[0] = (dev->speaker_enable_right << 7) |
		(dev->speaker_enable_left << 6) |
		//(dev->software_shutdown << 5) |
		(dev->noise_gate_enable);
	dev->raw[1] = dev->agc_attack_time;
	dev->raw[2] = dev->agc_release_time;
	dev->raw[3] = dev->agc_hold_time;
	dev->raw[4] = dev->agc_fixed_gain;
	dev->raw[5] = (dev->output_limiter_disable << 7) |
		(dev->noise_gate_threshold << 5) |
		(dev->output_limiter_level);
	dev->raw[6] = (dev->max_gain << 4) |
		(dev->compression_ratio);


	status = i2c_start(TPA2016_ADDRESS, TW_WRITE);
	if (status != 0) {
		i2c_stop();
		return status;
	}
	status = i2c_write(1);
	if (status != 0) {
		i2c_stop();
		return status;
	}
	for (i = 0; i < 7; i++) {
		i2c_write(dev->raw[i]);
	}
	i2c_stop();
	return 0;
}
