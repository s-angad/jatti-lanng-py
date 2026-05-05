#pragma once

#include "common.h"
#include "chunk.h"
#include "table.h"
#include "arena.h"

#define FRAMES_MAX 64
#define TRY_MAX 64

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    u8* ip;
    Table locals;
    bool is_script;
} CallFrame;

typedef struct {
    int frame_index;
    u8* handler_ip;
} TryHandler;

struct VM {
    Chunk* chunk;
    u8* ip;

    Value stack[STACK_MAX];
    Value* stack_top;

    CallFrame frames[FRAMES_MAX];
    int frame_count;

    TryHandler try_handlers[TRY_MAX];
    int try_count;

    Table globals;
    Table strings; // used for (optional) interning and as stable key store

    Obj* objects; // linked list for cleanup

    Arena scratch;

    int runtime_error_line;
    const char* runtime_error_msg;
};

typedef struct VM VM;

void vm_init(VM* vm);
void vm_free(VM* vm);

JattiStatus vm_run(VM* vm, Chunk* chunk);
