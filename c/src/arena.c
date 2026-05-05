#include "arena.h"

#include <stdlib.h>
#include <string.h>

static size_t align_up(size_t x, size_t align) {
    size_t mask = align - 1;
    return (x + mask) & ~mask;
}

void arena_init(Arena* arena) {
    arena->data = NULL;
    arena->used = 0;
    arena->capacity = 0;
}

void arena_free(Arena* arena) {
    free(arena->data);
    arena->data = NULL;
    arena->used = 0;
    arena->capacity = 0;
}

void* arena_alloc(Arena* arena, size_t size, size_t align) {
    if (align == 0) align = 1;
    size_t start = align_up(arena->used, align);
    size_t end = start + size;
    if (end > arena->capacity) {
        size_t new_cap = arena->capacity ? arena->capacity : 4096;
        while (new_cap < end) new_cap *= 2;
        void* new_data = realloc(arena->data, new_cap);
        if (!new_data) return NULL;
        arena->data = (u8*)new_data;
        arena->capacity = new_cap;
    }
    void* ptr = arena->data + start;
    arena->used = end;
    return ptr;
}

char* arena_strdup(Arena* arena, const char* s, size_t len) {
    char* out = (char*)arena_alloc(arena, len + 1, 1);
    if (!out) return NULL;
    memcpy(out, s, len);
    out[len] = '\0';
    return out;
}
