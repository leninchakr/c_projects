#include <unity.h>
#include <add.h>
#include <stdio.h>

/*
 * Title: Testing int add(int,int)
 *
 * Test Cases:
 * ----------
 *  1. Add two positive number
 *  2. Add two zeros
 *  3. Add two negative numbers
 *  4. Add one positive number & one negative number
*/

void setUp(void) {
    printf("Creating resources...\n");
    printf("\n");
}

void tearDown(void) {
    printf("Closing resources...\n");
    printf("\n");
}

/*
 * Test-1 : Adding two Positive Numbers
 *
 * Input: 2,3
 * Output: 5
*/
void test_add_two_positive_number(void) {

    int expected = 5;
    int actual = add(2,3);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

/*
 * Test-2: Add Two Zeros
 * 
 * Input: 0,0
 * Output: 0
*/
void test_add_two_zeros(void){
    
    int expected = 0;
    int actual = add(0,0);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

/*
 * Test-3: Add Two Negative Numbers
 *
 * Input: -4,-7
 * Output: -11
*/
void test_add_two_negative_numbers(void) {

    int expected = -11;
    int actual = add(-4,-7);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

/*
 * Test-4: Add one positive and one negative number
 *
 * Input: 5,-7
 * Output: -2
*/
void test_add_one_pos_one_neg_number(void) {

    int expected = -2;
    int actual = add(5,-7);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

int main(void) {

    UNITY_BEGIN();

    // Test-1
    RUN_TEST(test_add_two_positive_number);

    // Test-2
    RUN_TEST(test_add_two_zeros);

    // Test-3
    RUN_TEST(test_add_two_negative_numbers);

    // Test-4
    RUN_TEST(test_add_one_pos_one_neg_number);

    return UNITY_END();
}
