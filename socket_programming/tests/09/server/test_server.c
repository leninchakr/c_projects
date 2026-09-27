#include <unity.h>
#include <stdio.h>
#include <stddef.h>

#include "../../include/09/server/server.h"

void setUp(void)
{
    printf("Setting up resources...\n");
}

void tearDown(void)
{
    printf("Closing resources...\n");
}

/*
 *  Test-1: init_list()
 *
 *  Function: init_list()
 *  Input: Address of Linked-List
 *  Output: head=NULL, tail=NULL, count=0
 */
void test_main_init_list(void)
{

    MessageList input_ll = {
        .head = NULL,
        .tail = NULL,
        .count = 74
    };

    init_list(&input_ll);

    MessageList expected = {
        .head = NULL,
        .tail = NULL,
        .count = 0
    };

    TEST_ASSERT_NULL(input_ll.head);
    TEST_ASSERT_NULL(input_ll.tail);
    TEST_ASSERT_EQUAL_UINT(expected.count, input_ll.count);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_main_init_list);

    return UNITY_END();
}
