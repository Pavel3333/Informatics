#pragma once

// operations (sorted by priority)
typedef enum {
    OP_XOR,    // ^
    OP_SUB,    // -
    OP_ADD,    // +
    OP_REM,    // %
    OP_DIV,    // /
    OP_MUL,    // *
    OP_POW,    // **

    OP_COUNT   // count of operations
} OperationCode;

char* Operation[OP_COUNT];

// brackets
typedef enum {
    BR_OPEN,    // (
    BR_CLOSE,   // )

    BR_COUNT    // count of operations
} BracketCode;

char Bracket[BR_COUNT];