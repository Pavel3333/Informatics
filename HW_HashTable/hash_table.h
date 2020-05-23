#pragma once

#include "node.h"
#include "error.h"

#define HASH_TABLE_SIZE 256

Error hashTableAddStudent(Node* hash_table[HASH_TABLE_SIZE], Student* student);
Error hashTableAddRaw(
    Node* hash_table[HASH_TABLE_SIZE],
    char* first_name,
    char* last_name,
    Gender gender,
    float height,
    float weight,
    uint16_t group,
    float average_score
);
Error hashTableLookup(Node* hash_table[HASH_TABLE_SIZE], const char* key, Node** dest);
Error hashTableDel(   Node* hash_table[HASH_TABLE_SIZE], const char* key);

Error hashTableLoad(Node* hash_table[HASH_TABLE_SIZE], const char* path);
Error hashTableDump(Node* hash_table[HASH_TABLE_SIZE], const char* path);

void hashTablePrint(Node* hash_table[HASH_TABLE_SIZE]);