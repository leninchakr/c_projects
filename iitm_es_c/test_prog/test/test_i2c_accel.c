// SPDX-License-Identifier: GPL-2.0-only
#include "unity.h"
#include "i2c_accel.h"

void setUp(void)
{
	i2c_accel_init();
}

void tearDown(void)
{
}

void test_accel_read_x_axis_success(void)
{
	int16_t x_axis_value = 0;
	TEST_ASSERT_EQUAL_INT(0, i2c_accel_read_x(&x_axis_value));
	TEST_ASSERT_EQUAL_INT(512, x_axis_value);
}

void test_accel_read_x_null_pointer_protection(void)
{
	/* Ensure the driver doesn't crash if passed a NULL pointer */
	TEST_ASSERT_EQUAL_INT(-2, i2c_accel_read_x(NULL));
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_accel_read_x_axis_success);
	RUN_TEST(test_accel_read_x_null_pointer_protection);
	return UNITY_END();
}
