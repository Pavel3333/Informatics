#pragma once
#include "node.h"

// Структура стэка на односвязном списке
typedef struct {
    Node* head; // Первый элемент стека
    unsigned int size;
} Stack;

// Объявление
Stack* init_stack();
bool   empty_stack(Stack* stack);
void   list_stack(Stack* stack);
int    push_stack(Stack* stack, Value value, ValueType type);
int    pop_stack(Stack* stack, Node* dest);
void   fini_stack(Stack* stack);