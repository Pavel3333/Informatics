#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "stack_sized.h"
#include "main.h"

Stack* init_stack() {
    // Выделим память
    Stack* new_stack = (Stack*)calloc(sizeof(Stack), 1);
    if (!new_stack) {
        printf("Unable to allocate the memory for the stack object\n");
        return NULL;
    }

    return new_stack;
}

bool empty_stack(Stack* stack) {
    if (!stack)
        return true;
    return !(bool)(stack->head);
}

void list_stack(Stack* stack) {
    if (!stack) {
        printf("Stack object cannot be a NULL pointer for push\n");
        return;
    }

    Node* node = stack->head;

    printf("head -> ");
    while (node) {
        switch (node->type) {
        case T_OPERAND:
            printf("%lld -> ", node->value);
            break;
        case T_OPERATOR:
            printf("%s -> ", Operation[node->value]);
            break;
        case T_BRACKET:
            printf("%c -> ", Bracket[node->value]);
            break;
        }

        node = node->next;
    }
    printf("NULL\n");
}

int push_stack(Stack* stack, Value value, ValueType type) {
    if (!stack)
        return STACK_NULL_OBJECT;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node)
        return STACK_NEW_NODE;

    new_node->type  = type;
    new_node->value = value;
    new_node->next  = stack->head;

    stack->head = new_node;
    stack->size++;

#ifdef _DEBUG
    printf("Stack: pushed value is %lld (type: %d)\n", new_node->value, new_node->type);
#endif

    return SUCCESS;
}

int pop_stack(Stack* stack, Node* dest) {
    if (!stack)
        return STACK_NULL_OBJECT;
    else if (!stack->head)
        return STACK_EMPTY;

    Node old_head = *(stack->head);

    free(stack->head);
    stack->head = old_head.next;
    stack->size--;

#ifdef _DEBUG
    printf("Stack: popped value is %lld (type: %d)\n", old_head.value, old_head.type);
#endif

    if (dest)             // delete head element if dest == NULL
        *dest = old_head;
    return SUCCESS;
}

void fini_stack(Stack* stack) {
    if (stack)
        free(stack->head);

    free(stack);
}