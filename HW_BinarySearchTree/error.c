#include <stdio.h>

#include "error.h"

void handle_error(Error err) {
    if (is_warning(err))
        printf("Warning! ");
    else if (is_error(err))
        printf("Error! ");

    switch (err) {
    case E_Success:
        break;
    case W_KeyAlreadyExists:
        puts("Node already exists!");
        break;
    case W_NotFound:
        puts("Node was not found!");
        break;
    case E_NullArg:
        puts("Null argument exception");
        break;
    case E_MemoryAlloc:
        puts("Memory allocation failed");
        break;
    }
}