#pragma once

#include <stdint.h>

#include "error.h"

#define MAX 256
#define POLYNOMIAL 0x81

Error CRC8(char* string, uint8_t* hash);