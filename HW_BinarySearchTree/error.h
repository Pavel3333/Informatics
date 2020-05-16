#pragma once

#define is_warning(err) (err <  0)
#define is_success(err) (err == 0)
#define is_error(err)   (err >  0)

typedef enum {
    W_KeyAlreadyExists = -2,
    W_NotFound,

    E_Success,

    E_NullArg,
    E_MemoryAlloc
} Error;
static_assert(E_Success == 0, "Invalid enum declaration; please check start number");

void handle_error(Error err);