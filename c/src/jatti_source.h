#pragma once

#include "common.h"
#include "arena.h"

typedef struct {
    char* data;
    size_t length;
} Source;

JattiStatus source_read_file(Arena* arena, const char* path, Source* out);

// Split into lines (pointers into arena-owned copies).
typedef struct {
    const char** lines;
    int count;
} Lines;

JattiStatus source_split_lines(Arena* arena, Source src, Lines* out);

// Strip comments from a single line, respecting simple quotes.
// Supports both `#` and `fuddu_chiz` as comment starts.
JattiStatus source_strip_comments(Arena* arena, const char* line, const char** out);
