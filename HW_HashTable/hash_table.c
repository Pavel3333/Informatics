#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#include "io.h"
#include "CRC8.h"

// Internal functions. Trading safety for speed

static Error _hashTableLookup(Node* hash_table[HASH_TABLE_SIZE], const char* key, Node*** dest) {
    Error err = E_Success;
    uint8_t hash;

    err = CRC8((char*)key, &hash);
    if (is_error(err))
        return err;

    Node** node = &(hash_table[hash]);
    Node* temp;

    err = W_NodeNotFound;

    while (temp = *node) {
        // Check if that key already exists
        if (!strcmp(temp->key, key)) {
            err = E_Success;
            break;
        }

        node = &(temp->next);
    }

    *dest = node;
    return err;
}

// Public functions

Error hashTableAddStudent(Node* hash_table[HASH_TABLE_SIZE], Student* student) {
    if (!hash_table || !student)
        return E_NullArg;

    Error err;
    Node** temp;

    Node* new_node = malloc(sizeof(Node));
    if (!new_node)
        return E_MemoryAlloc;

    size_t key_size = strlen(student->first_name) + strlen(" ") + strlen(student->last_name) + 1;

    new_node->key = malloc(sizeof(char) * key_size);
    if (!new_node->key) {
        err = E_MemoryAlloc;
        goto _HTA_err_1;
    }
    new_node->key[0] = '\0';

#ifdef _WIN32
    strcat_s(new_node->key, key_size, student->first_name);
    strcat_s(new_node->key, key_size, " ");
    strcat_s(new_node->key, key_size, student->last_name);
#else
    strcat(new_node->key, first_name);
    strcat(new_node->key, " ");
    strcat(new_node->key, last_name);
#endif

    new_node->student = *student;
    new_node->next = NULL;

    err = _hashTableLookup(hash_table, new_node->key, &temp);
    if (is_error(err))
        goto _HTA_err_2;
    if (is_success(err)) {
        err = W_KeyAlreadyExists;
        goto _HTA_err_2;
    }
    if (is_warning(err)) {
        switch (err) {
        case W_NodeNotFound:
            break;
        default:
            goto _HTA_err_2;
        }
    }

    *temp = new_node;
    return E_Success;

    // Error occured
_HTA_err_2:
    free(new_node->key);
_HTA_err_1:
    free(new_node);
    return err;
}

Error hashTableAddRaw(
    Node* hash_table[HASH_TABLE_SIZE],
    char* first_name,
    char* last_name,
    Gender gender,
    float height,
    float weight,
    uint16_t group,
    float average_score
) {
    if (!hash_table || !first_name || !last_name)
        return E_NullArg;

    Error   err;
    Student student;

    err = newStudent(&student, first_name, last_name, gender, height, weight, group, average_score);
    if (!is_success(err))
        return err;

    err = hashTableAddStudent(hash_table, &student);
    if (!is_success(err))
        goto HTA_err_1;

    return E_Success;

    // Error occured
HTA_err_1:
    delStudent(&student);
    return err;
};

Error hashTableLookup(Node* hash_table[HASH_TABLE_SIZE], const char* key, Node** dest) {
    if (!hash_table || !key)
        return E_NullArg;

    Error err;
    Node** node;

    err = _hashTableLookup(hash_table, key, &node);
    if (!is_success(err))
        return err;
    
    if (dest)
        *dest = *node;

    return E_Success;
}

Error hashTableDel(Node* hash_table[HASH_TABLE_SIZE], const char* key) {
    if (!hash_table)
        return E_NullArg;

    Error err;
    Node** node;

    if (key) { // Delete node
        err = _hashTableLookup(hash_table, key, &node);
        if (!is_success(err))
            return err;

        Node* temp = *node;
        *node = temp->next;

        if (temp)
            delNode(temp);
    }
    else {     // Delete tree
        Node* temp;
        for (uint16_t hash = 0; hash < HASH_TABLE_SIZE; hash++) {
            node = &(hash_table[hash]);
            while (*node) {
                temp = (*node)->next;
                delNode(*node);
                *node = NULL;
                node = &temp;
            }
        }
    }

    return E_Success;
}

Error hashTableLoad(Node* hash_table[HASH_TABLE_SIZE], const char* path) {
    if (!hash_table || !path)
        return E_NullArg;

    Error err = E_Success;
    FILE* hash_table_file;

#ifdef _WIN32
    int io_err = fopen_s(&hash_table_file, path, "rb");
    if (io_err)
        return W_FileNotOpened;
#else
    FILE* hash_table_file = fopen(path, "rb");
    if (!hash_table_file)
        return W_FileNotOpened;
#endif

    Student student;

    while (!feof(hash_table_file)) {
        err = loadStudent(hash_table_file, &student);
        if (is_error(err))
            break;
        if (is_warning(err)) {
            if (err == W_EOF)
                err = E_Success; // ignoring EOF
            break;
        }

        err = hashTableAddStudent(hash_table, &student);
        if (!is_success(err))
            break;
    }

    fclose(hash_table_file);

    if (!is_error(err))
        puts("Hash table data was successfully loaded");

    return err;
}

Error hashTableDump(Node* hash_table[HASH_TABLE_SIZE], const char* path) {
    if (!hash_table || !path)
        return E_NullArg;

    Error err = E_Success;

    FILE* hash_table_file;

#ifdef _WIN32
    int io_err = fopen_s(&hash_table_file, path, "wb");
    if (io_err)
        return W_FileNotOpened;
#else
    FILE* hash_table_file = fopen(path, "wb");
    if (!hash_table_file)
        return W_FileNotOpened;
#endif

    for (uint16_t hash = 0; hash < HASH_TABLE_SIZE; hash++) {
        Node* node = hash_table[hash];
        if (!node)
            continue;

        err = dumpStudent(hash_table_file, &(node->student));
        if (!is_success(err))
            break;
    }

    fclose(hash_table_file);

    puts("Hash table data was successfully saved");

    return err;
}

void hashTablePrint(Node* hash_table[HASH_TABLE_SIZE]) {
    if (!hash_table)
        return;

    size_t printed = 0;

    puts("Table:");

    for (uint16_t hash = 0; hash < HASH_TABLE_SIZE; hash++) {
        Node* node = hash_table[hash];
        if (!node)
            continue;

        printf("%d. Hash: %d;\n", printed + 1, hash);
        while (node) {
            printNode(node, INDENT_SIZE);

            node = node->next;
        }

        printed++;
    }

    if (!printed)
        puts("Table is empty");
}