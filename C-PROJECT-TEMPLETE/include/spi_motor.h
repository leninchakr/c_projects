// SPDX-License-Identifier: GPL-2.0-only
#ifndef SPI_MOTOR_H
#define SPI_MOTOR_H

#include <stdint.h>

/* Mock SPI registers for the motor controller */
#define MOTOR_REG_SPEED  0x20
#define MOTOR_REG_STATUS 0x21

int spi_motor_init(void);
int spi_motor_set_speed(uint8_t speed);
int spi_motor_get_status(uint8_t *status);

#endif /* SPI_MOTOR_H */
