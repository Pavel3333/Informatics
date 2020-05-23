#pragma once

#include <stdint.h>

#include "error.h"

#define INDENT_SIZE 4

Error my_scanf(size_t argcount, const char* fmt, ...);
Error my_fgets(char* buffer, int max_count, FILE* stream);