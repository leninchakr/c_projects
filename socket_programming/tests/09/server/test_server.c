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

/*
 * Test-2: Add Node
 *
 * Function: add_node(&ll, "Sample Msg");
 * Input: &ll, "Sample Msg"
 * Output: A node in Linked-List
 *
 */
void test_add_node(void)
{
    MessageList ll = {0};

    init_list(&ll);

    int create_node_status = add_node(&ll, "Sample Msg");

    TEST_ASSERT_EQUAL_INT(0, create_node_status);

    // Test List
    TEST_ASSERT_NOT_NULL(ll.head);
    TEST_ASSERT_NOT_NULL(ll.tail);
    TEST_ASSERT_EQUAL_UINT(1, ll.count);

    // Test Node
    MessageNode *node_actual = ll.head;
    
    MessageNode node_expected = {
        .data = "Sample Msg",
        .next = NULL
    };

    TEST_ASSERT_EQUAL_STRING(node_expected.data, node_actual->data);
    TEST_ASSERT_NULL(node_actual->next);
    
}

/*
 * Test-3: Add 3 Nodes To Linked-List
 *
 * Function: add_node()
 * Input: &ll, "Message"
 * Output: 3 Nodes in the Linked-List
*/
void test_add_node_3_times(void)
{
    MessageList ll = {0};

    init_list(&ll);

    add_node(&ll, "Apple");
    add_node(&ll, "Ball");
    add_node(&ll, "Cat");

    /******************************/

    // Test Linked List
    int nodes_added = 3;
    TEST_ASSERT_EQUAL_UINT(nodes_added, ll.count);
    
    // Test Nodes
    MessageNode *first_node = ll.head;
    TEST_ASSERT_NOT_NULL(first_node);

    MessageNode *second_node = first_node->next;
    TEST_ASSERT_NOT_NULL(second_node);

    MessageNode *third_node = second_node->next;
    TEST_ASSERT_NOT_NULL(third_node);

    TEST_ASSERT_EQUAL_STRING("Apple", first_node->data);
    TEST_ASSERT_EQUAL_STRING("Ball", second_node->data);
    TEST_ASSERT_EQUAL_STRING("Cat", third_node->data);

    TEST_ASSERT_EQUAL_PTR(first_node, ll.head);
    TEST_ASSERT_EQUAL_PTR(second_node, first_node->next);
    TEST_ASSERT_EQUAL_PTR(third_node, second_node->next);
    TEST_ASSERT_EQUAL_PTR(third_node, ll.tail);
    TEST_ASSERT_NULL(third_node->next);

}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_main_init_list);
    RUN_TEST(test_add_node);
    RUN_TEST(test_add_node_3_times);

    return UNITY_END();
}
