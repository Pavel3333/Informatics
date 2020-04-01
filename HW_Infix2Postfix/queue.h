#pragma once
#include "node.h"

// ��������� ����� �� ����������� ������
typedef struct {
    Node* head;        // ������ ������� �������
    Node* leaf;        // ��������� ������� �������
    unsigned int size;
} Queue;

Queue* init_queue();
bool   empty_queue(Queue* queue);
void   list_queue(Queue* queue);
int    del_queue(Queue* queue, Value value, ValueType type);
int    push_queue(Queue* queue, Value value, ValueType type);
int    pop_queue(Queue* queue, Node* dest);
void   fini_queue(Queue* queue);