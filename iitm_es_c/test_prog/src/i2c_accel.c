// SPDX-License-Identifier: GPL-2.0-only
#include "i2c_accel.h"

static int is_i2c_initialized = 0;

int i2c_accel_init(void)
{
	is_i2c_initialized = 1;
	return 0;
}

int i2c_accel_read_x(int16_t *out_x)
{
	if (!is_i2c_initialized)
		return -1;

	if (!out_x)
		return -2; /* Error: Null pointer passed */

	/* Mocking a stable acceleration reading of 512 units */
	*out_x = 512; 
	return 0;
}
