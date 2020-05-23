#include <stdio.h>

#include "io.h"
#include "node.h"
#include "error.h"
#include "hash_table.h"
#include "interface.h"

#define DEFAULT_TABLE_PATH "hash_table.bin"

int main() {
    Error err;

    Node* hash_table[HASH_TABLE_SIZE] = { 0 };

    err = hashTableLoad(hash_table, DEFAULT_TABLE_PATH);
    process_err();

    printCommands();

    Command cmd;
    while (1) {
        printf("%s", "Please type the command: ");
        err = my_scanf(1, "%u", (unsigned int*)(&cmd));
        process_err();
        if (is_warning(err)) {
            if (err == W_FailScan)
                continue;
            else
                break;
        }

        switch (cmd) {
        case C_PrintCommands:
            printCommands();
            break;
        case C_Print:
            hashTablePrint(hash_table);
            break;
        case C_Add:
            addCommand(hash_table);
            break;
        case C_Lookup:
            lookupCommand(hash_table);
            break;
        case C_DeleteNode:
            deleteNodeCommand(hash_table);
            break;
        case C_DeleteTable:
            deleteTableCommand(hash_table);
            break;
        case C_Load:
            loadCommand(hash_table);
            break;
        case C_Dump:
            dumpCommand(hash_table);
            break;
        case C_Exit:
            break;
        default:
            puts("Unknown command");
        }

        if (cmd == C_Exit)
            break;
    }

    puts("Good bye!");
    return 0;
}