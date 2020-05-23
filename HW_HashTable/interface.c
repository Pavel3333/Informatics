#include <stdio.h>

#include "interface.h"

#include "io.h"
#include "node.h"
#include "error.h"

const char* CommandDesc[C_SIZE] = {
    "Print commands",
    "Print hash table",
    "Add item to hash table",
    "Find item in hash table by key",
    "Delete node from table by key",
    "Delete table",
    "Load table from the file",
    "Save data into the file",
    "Exit"
};

// Print functions

void printIndent(uint16_t indent) {
    for (uint16_t i = 0; i < indent; i++)
        putc(' ', stdout);
}

void printCommands() {
    puts("Commands: ");
    for (uint8_t i = 0; i < C_SIZE; i++) {
        printIndent(INDENT_SIZE);
        printf("%hhu. %s\n", i, CommandDesc[i]);
    }
}

// Command functions

void addCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err;
    Student student; // for POD fields
    char first_name[BUFFER_SIZE] = { 0 };
    char last_name[BUFFER_SIZE] = { 0 };
    char gender[BUFFER_SIZE] = { 0 };

    printf("%s", "Please enter the first name: ");
    err = my_fgets(first_name, BUFFER_SIZE, stdin);
    process_err_strict_void();

    printf("%s", "Please enter the last name: ");
    err = my_fgets(last_name, BUFFER_SIZE, stdin);
    process_err_strict_void();

    printf(
        "Please enter the gender (\"%s\" or \"%s\"): ",
        GenderDesc[G_Male],
        GenderDesc[G_Female]
    );

    while (1) {
        err = my_fgets(gender, BUFFER_SIZE, stdin);
        process_err_strict_void();

        err = getGenderByDesc(gender, &(student.gender));
        process_err_void();
        if (is_warning(err)) {
            if (err == W_GenderNotFound)
                puts("Invalid gender. Please retry");
            continue;
        }

        break;
    }

    printf("%s", "Please enter height: ");
    err = my_scanf(1, "%f", &(student.height));
    process_err_strict_void();

    printf("%s", "Please enter weight: ");
    err = my_scanf(1, "%f", &(student.weight));
    process_err_strict_void();

    printf("%s", "Please enter group: ");
    err = my_scanf(1, "%hu", &(student.group));
    process_err_strict_void();

    printf("%s", "Please enter average score: ");
    err = my_scanf(1, "%f", &(student.average_score));
    process_err_strict_void();

    err = hashTableAddRaw(
        hash_table,
        first_name,
        last_name,
        student.gender,
        student.height,
        student.weight,
        student.group,
        student.average_score
    );
    process_err_strict_void();

    puts("Node was successfully added");
}

void lookupCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err;
    char key[BUFFER_SIZE] = { 0 };
    Node* found;

    printf("%s", "Please enter the key to find: ");
    err = my_fgets(key, BUFFER_SIZE, stdin);
    process_err_strict_void();

    err = hashTableLookup(hash_table, key, &found);
    process_err_void();
    if (is_warning(err)) {
        if (err == W_NodeNotFound)
            puts("Node was not found");

        return;
    }

    puts("Found node:");
    printNode(found, INDENT_SIZE);
}

void deleteNodeCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err;
    char key[BUFFER_SIZE] = { 0 };

    printf("%s", "Please enter the key: ");
    err = my_fgets(key, BUFFER_SIZE, stdin);
    process_err_strict_void();

    err = hashTableDel(hash_table, key);
    process_err_strict_void();

    puts("Node was successfully deleted");
}

void deleteTableCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err = hashTableDel(hash_table, NULL);
    process_err_strict_void();

    puts("Table was successfully deleted");
}

void loadCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err;
    char path[BUFFER_SIZE] = { 0 };

    printf("%s", "Please enter the filename: ");
    err = my_fgets(path, BUFFER_SIZE, stdin);
    process_err_strict_void();

    err = hashTableLoad(hash_table, path);
    process_err_strict_void();
}

void dumpCommand(Node* hash_table[HASH_TABLE_SIZE]) {
    Error err;
    char path[BUFFER_SIZE] = { 0 };

    printf("%s", "Please enter the filename: ");
    err = my_fgets(path, BUFFER_SIZE, stdin);
    process_err_strict_void();

    err = hashTableDump(hash_table, path);
    process_err_strict_void();
}