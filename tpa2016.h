#ifndef __TPA2016_H
#define __TPA2016_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

#define TPA2016_ADDRESS 0x58

#define TPA2016_COMPRESSION_1_1 0;
#define TPA2016_COMPRESSION_2_1 1;
#define TPA2016_COMPRESSION_4_1 2;
#define TPA2016_COMPRESSION_8_1 3;

#define TPA2016_MAX_GAIN_BASE_DB 18

typedef struct {
	uint8_t raw[8];

	// Register 01 (function control)

	bool speaker_enable_left;
	bool speaker_enable_right;
	bool software_shutdown;
	// Overcurrent event on left channel happened
	bool fault_left;
	// Overcurrent event on right channel happened
	bool fault_right;
	// Thermal shutdown was initiated
	bool thermal_shutdown;
	// Enable noise gate (only compression off, i.e. 1:1)
	bool noise_gate_enable;
	
	// Register 02 (agc attack control)
	uint8_t agc_attack_time;

	// Register 03 (agc release control)
	uint8_t agc_release_time;

	// Register 04 (agc hold time control)
	uint8_t agc_hold_time;

	// Register 05 (agc fixed gain control)
	uint8_t agc_fixed_gain;

	// Register 06 (agc control)
	bool output_limiter_disable;
	uint8_t noise_gate_threshold;
	uint8_t output_limiter_level;

	// Register 07 (agc control)
	uint8_t max_gain;
	uint8_t compression_ratio;
} tpa2016_control;

int	tpa2016_init(tpa2016_control *dev);
int	tpa2016_get(tpa2016_control *dev);
int	tpa2016_set(tpa2016_control *dev);

#endif
