#include "CRC8.h"

Error CRC8(char* string, uint8_t* hash)
{
    if (!string)
        return E_NullArg;

    uint8_t crc = 0x00;
    char symbol;

    while (symbol = *string)
    {
        crc ^= symbol;

        for (int bit = 8; bit > 0; --bit)
        {
            crc = (crc << 1);
            if (crc & 0x80)
                crc ^= POLYNOMIAL;
        }

        string++;
    }

    *hash = crc;
    return E_Success;
}