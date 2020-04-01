#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "main.h"
#include "error.h"
#include "queue.h"

/*
head -> NULL
         ^
        leaf

head -> Node1 -> NULL
         ^
        leaf

head -> Node2 -> Node1 -> NULL
                  ^
                 leaf
*/

Queue* init_queue() {
    // Выделим память
    Queue* new_queue = (Queue*)calloc(1, sizeof(Queue));
    if (!new_queue) {
        printf("Unable to allocate the memory for the queue object\n");
        return NULL;
    }

    return new_queue;
}

bool empty_queue(Queue* queue) {
    if (!queue)
        return true;
    return !(bool)(queue->head);
}

void list_queue(Queue* queue) {
    if (!queue) {
        printf("Queue object cannot be a NULL pointer for list\n");
        return;
    }

    Node* node = queue->head;
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

int del_queue(Queue* queue, Value value, ValueType type) {
    if (!queue)
        return QUEUE_NULL_OBJECT;

    if (queue->head)
        if (queue->head->type == type && queue->head->value == value) {
            Node* next = queue->head->next;
            free(queue->head);
            queue->head = next;
            return SUCCESS;
        }

    Node* node = queue->head;
    
    while (node) {
        if (!node->next) break;

        if (node->next->type == type && node->next->value == value) {
            Node* next = node->next->next;
            free(node->next);
            node->next = next;
            return SUCCESS;
        }

        node = node->next;
    }

    return QUEUE_DEL_NOT_FOUND;
}

int push_queue(Queue* queue, Value value, ValueType type) {
    if (!queue)
        return QUEUE_NULL_OBJECT;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node)
        return QUEUE_NEW_NODE;

    new_node->type  = type;
    new_node->value = value;
    new_node->next = NULL;

    if (!queue->head)
        queue->head = new_node;

    if (queue->leaf)
        queue->leaf->next = new_node;

    queue->leaf = new_node;
    queue->size++;

#ifdef _DEBUG
    printf("Queue: pushed value is %lld (type: %d)\n", new_node->value, new_node->type);
#endif

    return SUCCESS;
}

int pop_queue(Queue* queue, Node* dest) {
    if (!queue)
        return QUEUE_NULL_OBJECT;
    else if (!queue->head)
        return QUEUE_EMPTY;

    Node old_head = *(queue->head);

    free(queue->head);
    queue->head = old_head.next;
    if (!queue->head)
        queue->leaf = NULL;

    queue->size--;

#ifdef _DEBUG
    printf("Queue: popped value is %lld (type: %d)\n", old_head.value, old_head.type);
#endif

    if (dest)             // delete head element if dest == NULL
        *dest = old_head;

    return SUCCESS;
}

void fini_queue(Queue* queue) {
    if (queue) {
        while (queue->head != NULL) {
#pragma warning(disable:6001)
            Node* next = queue->head->next;
#pragma warning(default:6001)
            free(queue->head);
            queue->head = next;
        }
    }

    free(queue);
}
/*
int main() {
    printf("Creating my own queue object...\n");
    Queue* my_queue = init_queue();
    if (!my_queue) {
        printf("Oh godness my queue was not initialised =(\n");
        return 1;
    }

    push_queue(my_queue, 777);
    push_queue(my_queue, 123);
    pop_queue(my_queue);
    push_queue(my_queue, 321);
    printf("Size of queue is %u\n", my_queue->size);
    pop_queue(my_queue);
    pop_queue(my_queue);

    printf("Let's delete the queue!\n");
    fini_queue(my_queue);
    my_queue = NULL;

    return 0;
}*/