#pragma once

typedef int64_t Value;

typedef enum {
    T_OPERAND,
    T_OPERATOR,
    T_BRACKET,

    T_COUNT
} ValueType;

typedef struct node {
    ValueType type;
    Value     value;

    struct node* next;
} Node;