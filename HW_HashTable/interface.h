#pragma once

#include "hash_table.h"

#define BUFFER_SIZE 256

#define process_err()       \
    if (!is_success(err)) { \
        handle_error(err);  \
        if (is_error(err))  \
            return err;     \
    }

#define process_err_void()  \
    if (!is_success(err)) { \
        handle_error(err);  \
        if (is_error(err))  \
            return;         \
    }

#define process_err_strict() \
    if (!is_success(err)) {     \
        handle_error(err);      \
        return err;             \
    }

#define process_err_strict_void() \
    if (!is_success(err)) {       \
        handle_error(err);        \
        return;                   \
    }

typedef enum {
    C_PrintCommands,
    C_Print,
    C_Add,
    C_Lookup,
    C_DeleteNode,
    C_DeleteTable,
    C_Load,
    C_Dump,
    C_Exit,

    C_SIZE
} Command;

const char* CommandDesc[C_SIZE];

void printIndent(uint16_t indent);
void printCommands();

void addCommand(Node * hash_table[HASH_TABLE_SIZE]);
void lookupCommand(Node* hash_table[HASH_TABLE_SIZE]);
void deleteNodeCommand(Node* hash_table[HASH_TABLE_SIZE]);
void deleteTableCommand(Node* hash_table[HASH_TABLE_SIZE]);
void loadCommand(Node* hash_table[HASH_TABLE_SIZE]);
void dumpCommand(Node* hash_table[HASH_TABLE_SIZE]);