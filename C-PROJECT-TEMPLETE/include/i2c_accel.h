// SPDX-License-Identifier: GPL-2.0-only
#ifndef I2C_ACCEL_H
#define I2C_ACCEL_H

#include <stdint.h>

/* Mock I2C Slave Address and Registers for Accelerometer */
#define ACCEL_I2C_ADDR   0x1D
#define ACCEL_REG_DATA_X 0x32
#define ACCEL_REG_WHO_AM_I 0x0F

int i2c_accel_init(void);
int i2c_accel_read_x(int16_t *out_x);

#endif /* I2C_ACCEL_H */
