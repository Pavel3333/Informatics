#pragma once

#define is_warning(err) (err <  0)
#define is_success(err) (err == 0)
#define is_error(err)   (err >  0)

typedef enum {
    W_EOF = -7,
    W_FailScan,
    W_NodeNotFound,
    W_FailReadInput,
    W_FileNotOpened,
    W_GenderNotFound,
    W_KeyAlreadyExists,

    E_Success,

    E_NullArg,
    E_MemoryAlloc,
    E_InvalidStudentFields
} Error;

#if _WIN32
#define my_static_assert static_assert
#else
#define my_static_assert _Static_assert
#endif

my_static_assert(E_Success == 0, "Invalid enum declaration; please check start number");

void handle_error(Error err);