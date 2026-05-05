#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Keep this project C11 + MSVC-friendly.

#define JATTI_UNUSED(x) (void)(x)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// A tiny status type.
typedef enum {
    JATTI_OK = 0,
    JATTI_ERR = 1,
} JattiStatus;
