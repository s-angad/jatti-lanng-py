#pragma once

#include "common.h"
#include "chunk.h"
#include "vm.h"

typedef struct {
    int line;
    const char* message;
} CompileError;

JattiStatus jatti_compile_file(VM* vm, const char* path, Chunk* out_chunk, CompileError* out_err);
