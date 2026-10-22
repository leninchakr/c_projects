// SPDX-License-Identifier: GPL-2.0-only
#include "unity.h"
#include "spi_motor.h"

void setUp(void)
{
	/* Automatically re-initialize driver state before every test */
	spi_motor_init();
}

void tearDown(void)
{
}

void test_motor_set_speed_valid_range(void)
{
	TEST_ASSERT_EQUAL_INT(0, spi_motor_set_speed(50));
	TEST_ASSERT_EQUAL_INT(0, spi_motor_set_speed(100));
}

void test_motor_set_speed_out_of_bounds(void)
{
	/* Verify that values above 100% fail properly */
	TEST_ASSERT_EQUAL_INT(-2, spi_motor_set_speed(105));
}

void test_motor_get_status_reads_correctly(void)
{
	uint8_t status_byte = 0;
	TEST_ASSERT_EQUAL_INT(0, spi_motor_get_status(&status_byte));
	TEST_ASSERT_EQUAL_INT(0x01, status_byte);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_motor_set_speed_valid_range);
	RUN_TEST(test_motor_set_speed_out_of_bounds);
	RUN_TEST(test_motor_get_status_reads_correctly);
	return UNITY_END();
}
