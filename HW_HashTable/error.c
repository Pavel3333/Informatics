#include <stdio.h>

#include "error.h"

void handle_error(Error err) {
    if (is_warning(err))
        printf("Warning! ");
    else if (is_error(err))
        printf("Error! ");

    switch (err) {
    case W_EOF:
        puts("EOF while reading the file");
        break;
    case W_FailScan:
        puts("Input scanning failed");
        break;
    case W_NodeNotFound:
        puts("Node was not found");
        break;
    case W_FailReadInput:
        puts("Fail to read string from input");
        break;
    case W_FileNotOpened:
        puts("File was not opened");
        break;
    case W_GenderNotFound:
        puts("Gender was not found");
        break;
    case W_KeyAlreadyExists:
        puts("Key already exists");
        break;
    case E_Success:
        break;
    case E_NullArg:
        puts("Null argument exception");
        break;
    case E_MemoryAlloc:
        puts("Memory allocation failed");
        break;
    case E_InvalidStudentFields:
        puts("Invalid student field");
        break;
    }
}