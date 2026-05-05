#pragma once

#include "common.h"
#include "expr_scan.h"
#include "chunk.h"
#include "vm.h"

// Compile an expression string to bytecode that leaves a value on the stack.
// Returns JATTI_OK/JATTI_ERR. On error, `err_msg` points to a static string.
JattiStatus expr_compile(VM* vm, Chunk* chunk, const char* expr_src, int line, const char** err_msg);
