#pragma once

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONST,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,

    OP_GET_GLOBAL,
    OP_SET_GLOBAL,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,

    OP_NEG,
    OP_NOT,

    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_LTE,
    OP_GT,
    OP_GTE,

    OP_LIST,
    OP_DICT,
    OP_GET_INDEX,
    OP_LEN,

    OP_CALL,
    OP_RETURN,

    OP_TRY,
    OP_END_TRY,

    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_LOOP,

    OP_PRINT,
    OP_HALT,
} OpCode;

typedef struct {
    int count;
    int capacity;
    u8* code;
    int* lines;

    int constants_count;
    int constants_capacity;
    Value* constants;
} Chunk;

void chunk_init(Chunk* chunk);
void chunk_free(Chunk* chunk);

void chunk_write(Chunk* chunk, u8 byte, int line);
int chunk_add_const(Chunk* chunk, Value value);

void chunk_write_u16(Chunk* chunk, u16 value, int line);
