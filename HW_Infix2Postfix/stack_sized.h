#pragma once
#include "node.h"

// ��������� ����� �� ����������� ������
typedef struct {
    Node* head; // ������ ������� �����
    unsigned int size;
} Stack;

// ����������
Stack* init_stack();
bool   empty_stack(Stack* stack);
void   list_stack(Stack* stack);
int    push_stack(Stack* stack, Value value, ValueType type);
int    pop_stack(Stack* stack, Node* dest);
void   fini_stack(Stack* stack);