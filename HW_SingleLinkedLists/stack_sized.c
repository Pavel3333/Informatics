#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node* next;
} Node;

// Структура стэка на односвязном списке
typedef struct {
    Node* head; // Первый элемент стека
    unsigned int size;
} Stack;

// Объявление
Stack* init_stack();
void push(Stack* stack, int value);
int pop(Stack* stack);
void fini_stack(Stack* stack);

Stack* init_stack() {
    // Выделим память
    Stack* new_stack = (Stack*)malloc(sizeof(Stack));
    if (!new_stack) {
        printf("Unable to allocate the memory for the stack object\n");
        return NULL;
    }

    // Стэк изначально пуст
    new_stack->head = NULL;
    new_stack->size = 0;

    // TODO: имеет смысл использовать calloc вместо malloc (все значения нулевые)

    return new_stack;
}

void push(Stack* stack, int value) {
    if (!stack) {
        printf("Stack object cannot be a NULL pointer for push\n");
        return;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Unable to allocate memory for the stack node\n");
        return;
    }

    new_node->value = value;
    new_node->next = stack->head;

    stack->head = new_node;
    stack->size++;

    printf("Pushed value is %d\n", new_node->value);
}

int pop(Stack* stack) {
    if (!stack) {
        printf("Stack object cannot be a NULL pointer\n");
        return 0;
    }
    else if (!stack->head) {
        printf("Stack head node cannot be a NULL pointer\n");
        return 0;
    }

    Node old_head = *(stack->head);

    free(stack->head);
    stack->head = old_head.next;
    stack->size--;

    printf("Popped value is %d\n", old_head.value);

    return old_head.value;
}

void fini_stack(Stack* stack) {
    if (stack)
        free(stack->head);

    free(stack);
}

int main() {
    printf("Creating my own stack object...\n");
    Stack* my_stack = init_stack();
    if (!my_stack) {
        printf("Oh godness my stack was not initialised =(\n");
        return 1;
    }

    push(my_stack, 777);
    push(my_stack, 123);
    pop(my_stack);
    push(my_stack, 321);
    printf("Size of stack is %u\n", my_stack->size);
    pop(my_stack);
    pop(my_stack);

    printf("Let's delete the stack!\n");
    fini_stack(my_stack);
    my_stack = NULL;

    return 0;
}