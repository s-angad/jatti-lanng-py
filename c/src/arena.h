#pragma once

#include "common.h"

typedef struct Arena {
    u8* data;
    size_t used;
    size_t capacity;
} Arena;

void arena_init(Arena* arena);
void arena_free(Arena* arena);
void* arena_alloc(Arena* arena, size_t size, size_t align);
char* arena_strdup(Arena* arena, const char* s, size_t len);
