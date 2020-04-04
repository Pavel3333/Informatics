#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "error.h"
#include "queue.h"
#include "stack_sized.h"

Queue* out   = NULL;
Stack* stack = NULL;

char* Operation[OP_COUNT] = {
    "^",
    "-",
    "+",
    "%",
    "/",
    "*",
    "**"
};

char Bracket[BR_COUNT] = {
    '(',
    ')'
};

bool is_sign(char input) {
    return (input == '-' || input == '+');
}

bool is_number(char input) {
    return (input >= '0' && input <= '9');
}

bool is_operand(char* input, unsigned int size) {
    if (is_sign(input[0])) { // ignore sign
        input++;
        size--;
    }

    bool flag = false;
    for (unsigned int i = 0; i < size; i++) {
        if (is_number(input[i]))
            flag = true;
        else {
            flag = false;
            break;
        }
    }
    return flag;
}

int8_t operation(char* input, unsigned int size) {
    if (size > 0 && size <= 2) {
        for (int8_t i = 0; i < OP_COUNT; i++) {
            if (!strcmp(input, Operation[i]))
                return i;
        }
    }
    
    return -1;
}

int8_t bracket(char* input, unsigned int size) {
    if (size == 1) {
        for (int8_t i = 0; i < BR_COUNT; i++) {
            if (input[0] == Bracket[i])
                return i;
        }
    }

    return -1;
}

int process_buf(char buf[32], unsigned int size) {
    int error = SUCCESS;
    int8_t code;
    Node buf_node;

    if (is_operand(buf, size))
        return push_queue(out, (Value)(atoi(buf)), T_OPERAND);
    else if ((code = operation(buf, size)) != -1) {
        while (!empty_stack(stack)) {
            if (stack->head->type != T_OPERATOR || (OperationCode)(stack->head->value) < code)
                break;
            error = pop_stack(stack, &buf_node);
            if (error)
                return error;

            error = push_queue(out, buf_node.value, buf_node.type);
            if (error)
                return error;
        }

        return push_stack(stack, (Value)(code), T_OPERATOR);
    }
    else if ((code = bracket(buf, size)) != -1) {
        if (code == BR_OPEN)
            return push_stack(stack, (Value)(code), T_BRACKET);
        else if (code == BR_CLOSE) {
            while (1) {
                error = pop_stack(stack, &buf_node);
                switch (error) {
                case SUCCESS:
                    break;
                case STACK_EMPTY:
                    return MAIN_NOT_ENOUGH_BRACKETS;
                default:
                    return error;
                }

                if (buf_node.type == T_BRACKET && buf_node.value == (Value)(BR_OPEN))
                    break;

                if (buf_node.type == T_OPERATOR) {
                    error = push_queue(out, buf_node.value, buf_node.type);
                    if (error)
                        return error;
                }
            }

            return error;
        }
        else
            return MAIN_UNKNOWN_BRACKET;
    }
    else
        return MAIN_UNKNOWN_TOKEN;
}

int process_infix(char* infix) {
    int error = SUCCESS;

    printf("Infix expression: %s\n", infix);

    char buf[32];
    Node buf_node;

    out = init_queue();
    if (!out) {
        error = MAIN_INIT_QUEUE;
        goto ret;
    }

    stack = init_stack();
    if (!stack) {
        error = MAIN_INIT_STACK;
        goto ret1;
    }

    char* ptr = infix;
    unsigned int counter = 0;
    while (!error) {
        if (*ptr == ' ' || *ptr == 0) {
            buf[counter] = 0;
            if (counter) { // process if token is not empty
                error = process_buf(buf, counter);
                if (error) goto ret2;
            }
            counter = 0;
        }
        else
            buf[counter++] = *ptr;

        if (*ptr) ptr++;
        else break;
    }

    printf("Stack    : ");
    list_stack(stack);
    printf("Out queue: ");
    list_queue(out);

    while (!empty_stack(stack)) {
        error = pop_stack(stack, &buf_node);
        if (error) goto ret2;

        if (buf_node.type == T_BRACKET) {
            error = MAIN_BRACKETS_NOT_CLOSED;
            goto ret2;
        }

        error = push_queue(out, buf_node.value, buf_node.type);
        if (error) goto ret2;
    }

    printf("Postfix expression: ");
    while (!empty_queue(out)) {
        error = pop_queue(out, &buf_node);
        if (error) goto ret2;

        if (buf_node.type == T_OPERAND)
            printf("%lld ", buf_node.value);
        else if (buf_node.type == T_OPERATOR)
            printf("%s ", Operation[buf_node.value]);
        else if (buf_node.type == T_BRACKET)
            printf("%c ", Bracket[buf_node.value]);
    }
    putc('\n', stdout);

ret2:
    fini_stack(stack);
ret1:
    fini_queue(out);
ret:
    return error;
}

#define TESTS_COUNT 5

int main() {
    int error = SUCCESS;

    char* tests[TESTS_COUNT] = {
        "7 - 2 * 3",                 // result must be "7 2 3 * −"
        "( 7 - 2 ) * 3",             // result must be "7 2 - 3 *"
        "7 - ( 2 * 3 )",             // result must be "7 2 3 * −"
        "7 - ( ( ( ( 2 * 3 ) ) ) )", // result must be "7 2 3 * −"
        "7 - ) 2 * 3 ("              // result must be ERROR (invalid brackets order)
    };

    for (int i = 0; i < TESTS_COUNT; i++) {
        printf("---------TEST %d---------\n", i);

        error = process_infix(tests[i]);

        switch (error) {
        case SUCCESS:
            printf("Success!\n");
            break;

        case MAIN_INIT_QUEUE:
            printf("Unable to create out queue\n");
            break;
        case MAIN_INIT_STACK:
            printf("Unable to create stack\n");
            break;
        case MAIN_NOT_ENOUGH_BRACKETS:
            printf("Syntax error: not enough open brackets\n");
            break;
        case MAIN_BRACKETS_NOT_CLOSED:
            printf("Syntax error: some brackets were not closed\n");
            break;
        case MAIN_UNKNOWN_BRACKET:
            printf("Found unknown bracket");
            break;
        case MAIN_UNKNOWN_TOKEN:
            printf("Found unknown token");
            break;

        case QUEUE_NULL_OBJECT:
            printf("Queue object cannot be a NULL pointer\n");
            break;
        case QUEUE_EMPTY:
            printf("Queue head cannot be a NULL pointer\n");
            break;
        case QUEUE_NEW_NODE:
            printf("Unable to allocate memory for the queue node\n");
            break;
        case QUEUE_DEL_NOT_FOUND:
            printf("Element was not found while deleting\n");
            break;

        case STACK_NULL_OBJECT:
            printf("Stack object cannot be a NULL pointer\n");
            break;
        case STACK_EMPTY:
            printf("Stack head node cannot be a NULL pointer\n");
            break;
        case STACK_NEW_NODE:
            printf("Unable to allocate memory for the stack node\n");
            break;
        }
    }

    return 0;
}