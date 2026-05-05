#include "jatti_source.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

JattiStatus source_read_file(Arena* arena, const char* path, Source* out) {
    FILE* f = fopen(path, "rb");
    if (!f) return JATTI_ERR;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz < 0) { fclose(f); return JATTI_ERR; }

    char* buf = (char*)arena_alloc(arena, (size_t)sz + 1, 1);
    if (!buf) { fclose(f); return JATTI_ERR; }

    size_t n = fread(buf, 1, (size_t)sz, f);
    fclose(f);
    buf[n] = '\0';

    out->data = buf;
    out->length = n;
    return JATTI_OK;
}

JattiStatus source_split_lines(Arena* arena, Source src, Lines* out) {
    // Copy line pointers into arena.
    int cap = 128;
    const char** lines = (const char**)arena_alloc(arena, (size_t)cap * sizeof(char*), sizeof(void*));
    if (!lines) return JATTI_ERR;

    int count = 0;
    const char* s = src.data;
    const char* start = s;

    for (size_t i = 0; i <= src.length; i++) {
        char c = src.data[i];
        if (c == '\n' || c == '\0') {
            size_t len = (size_t)(src.data + i - start);
            // Trim trailing \r
            if (len > 0 && start[len - 1] == '\r') len--;
            char* lineCopy = arena_strdup(arena, start, len);
            if (!lineCopy) return JATTI_ERR;

            if (count >= cap) {
                // grow: allocate new pointer array and copy
                int newCap = cap * 2;
                const char** newLines = (const char**)arena_alloc(arena, (size_t)newCap * sizeof(char*), sizeof(void*));
                if (!newLines) return JATTI_ERR;
                memcpy((void*)newLines, (void*)lines, (size_t)cap * sizeof(char*));
                lines = newLines;
                cap = newCap;
            }

            lines[count++] = lineCopy;
            start = src.data + i + 1;
        }
    }

    out->lines = lines;
    out->count = count;
    return JATTI_OK;
}

static bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static bool starts_with_word(const char* s, const char* word) {
    size_t wl = strlen(word);
    size_t sl = strlen(s);
    if (sl < wl) return false;
    if (strncmp(s, word, wl) != 0) return false;
    // boundary: end or whitespace
    char next = s[wl];
    return next == '\0' || is_space(next);
}

JattiStatus source_strip_comments(Arena* arena, const char* line, const char** out) {
    // Remove `# ...` and `fuddu_chiz ...` but not inside quotes.
    bool in_single = false;
    bool in_double = false;
    bool escape = false;

    size_t n = strlen(line);
    char* buf = (char*)arena_alloc(arena, n + 1, 1);
    if (!buf) return JATTI_ERR;

    size_t w = 0;
    for (size_t i = 0; i < n; i++) {
        char c = line[i];

        if (escape) {
            buf[w++] = c;
            escape = false;
            continue;
        }

        if ((in_single || in_double) && c == '\\') {
            buf[w++] = c;
            escape = true;
            continue;
        }

        if (!in_single && c == '"') {
            in_double = !in_double;
            buf[w++] = c;
            continue;
        }
        if (!in_double && c == '\'') {
            in_single = !in_single;
            buf[w++] = c;
            continue;
        }

        if (!in_single && !in_double) {
            if (c == '#') {
                break;
            }
            // Check fuddu_chiz at a word boundary (only if we are at a boundary too).
            if ((i == 0 || is_space(line[i - 1])) && starts_with_word(&line[i], "fuddu_chiz")) {
                break;
            }
        }

        buf[w++] = c;
    }

    // rtrim
    while (w > 0 && (buf[w - 1] == ' ' || buf[w - 1] == '\t')) w--;
    buf[w] = '\0';

    *out = buf;
    return JATTI_OK;
}
