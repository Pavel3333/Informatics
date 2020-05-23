#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "io.h"

// Wrappers

Error my_scanf(size_t argcount, const char* fmt, ...) {
    if (!fmt)
        return E_NullArg;

    int readed;

    va_list args;
    va_start(args, fmt);

//#ifdef _WIN32
//    readed = vscanf_s(fmt, args);
//#else
    readed = vscanf(fmt, args);
//#endif

    getchar();
    va_end(args);

    if (readed != argcount)
        return W_FailScan;

    return E_Success;
}

Error my_fgets(char* buffer, int max_count, FILE* stream) {
    char* result = fgets(buffer, max_count, stream);
    if (!result)
        return W_FailScan;

    result[strlen(result) - 1] = '\0'; // Replace '\n' to NULL-terminator

    return E_Success;
}