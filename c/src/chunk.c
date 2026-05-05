#include "chunk.h"

#include <stdlib.h>

static void* grow_array(void* ptr, size_t oldCount, size_t newCount, size_t itemSize) {
    JATTI_UNUSED(oldCount);
    return realloc(ptr, newCount * itemSize);
}

void chunk_init(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    chunk->constants_count = 0;
    chunk->constants_capacity = 0;
    chunk->constants = NULL;
}

void chunk_free(Chunk* chunk) {
    free(chunk->code);
    free(chunk->lines);
    free(chunk->constants);
    chunk_init(chunk);
}

void chunk_write(Chunk* chunk, u8 byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCap = chunk->capacity;
        int newCap = oldCap < 8 ? 8 : oldCap * 2;
        chunk->code = (u8*)grow_array(chunk->code, oldCap, newCap, sizeof(u8));
        chunk->lines = (int*)grow_array(chunk->lines, oldCap, newCap, sizeof(int));
        chunk->capacity = newCap;
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void chunk_write_u16(Chunk* chunk, u16 value, int line) {
    chunk_write(chunk, (u8)((value >> 8) & 0xFF), line);
    chunk_write(chunk, (u8)(value & 0xFF), line);
}

int chunk_add_const(Chunk* chunk, Value value) {
    if (chunk->constants_capacity < chunk->constants_count + 1) {
        int oldCap = chunk->constants_capacity;
        int newCap = oldCap < 8 ? 8 : oldCap * 2;
        chunk->constants = (Value*)grow_array(chunk->constants, oldCap, newCap, sizeof(Value));
        chunk->constants_capacity = newCap;
    }
    chunk->constants[chunk->constants_count] = value;
    return chunk->constants_count++;
}
