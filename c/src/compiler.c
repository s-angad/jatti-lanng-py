#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jatti_source.h"
#include "expr_parse.h"
#include "object.h"

static bool is_blank(const char* s) {
    while (*s) {
        if (*s != ' ' && *s != '\t' && *s != '\r' && *s != '\n') return false;
        s++;
    }
    return true;
}

static int indent_level(const char* s) {
    int spaces = 0;
    while (*s == ' ') { spaces++; s++; }
    return spaces / 4;
}

static const char* ltrim(const char* s) {
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

static bool starts_with(const char* s, const char* kw) {
    size_t n = strlen(kw);
    if (strncmp(s, kw, n) != 0) return false;
    char next = s[n];
    return next == '\0' || next == ' ' || next == '\t' || next == '(';
}

static void emit_const_name(VM* vm, Chunk* chunk, const char* name, int nameLen, int line, u16* outConstIdx) {
    ObjString* s = table_intern_string(&vm->strings, name, nameLen);
    int idx = chunk_add_const(chunk, OBJ_VAL(s));
    *outConstIdx = (u16)idx;
    JATTI_UNUSED(line);
}

static void emit_op_u16(Chunk* chunk, OpCode op, u16 operand, int line) {
    chunk_write(chunk, (u8)op, line);
    chunk_write_u16(chunk, operand, line);
}

static void emit_get_global(VM* vm, Chunk* chunk, const char* name, int nameLen, int line) {
    u16 nameConst = 0;
    emit_const_name(vm, chunk, name, nameLen, line, &nameConst);
    emit_op_u16(chunk, OP_GET_GLOBAL, nameConst, line);
}

static void emit_number_const(Chunk* chunk, double n, int line) {
    int idx = chunk_add_const(chunk, NUMBER_VAL(n));
    chunk_write(chunk, OP_CONST, line);
    chunk_write_u16(chunk, (u16)idx, line);
}

static u16 emit_jump(Chunk* chunk, OpCode op, int line) {
    chunk_write(chunk, (u8)op, line);
    chunk_write_u16(chunk, 0xFFFF, line);
    return (u16)(chunk->count - 2);
}

static u16 emit_try(Chunk* chunk, int line) {
    chunk_write(chunk, OP_TRY, line);
    u16 offPos = (u16)chunk->count;
    chunk_write_u16(chunk, 0xFFFF, line);
    return offPos;
}

static void patch_jump(Chunk* chunk, u16 offsetPos) {
    // offsetPos points to the first byte of the u16 operand
    int jumpFrom = (int)offsetPos + 2;
    int jumpTo = chunk->count;
    int off = jumpTo - jumpFrom;
    if (off < 0) off = 0;
    u16 uoff = (u16)off;
    chunk->code[offsetPos] = (u8)((uoff >> 8) & 0xFF);
    chunk->code[offsetPos + 1] = (u8)(uoff & 0xFF);
}

static void patch_jump_to(Chunk* chunk, u16 offsetPos, int jumpTo) {
    int jumpFrom = (int)offsetPos + 2;
    int off = jumpTo - jumpFrom;
    if (off < 0) off = 0;
    u16 uoff = (u16)off;
    chunk->code[offsetPos] = (u8)((uoff >> 8) & 0xFF);
    chunk->code[offsetPos + 1] = (u8)(uoff & 0xFF);
}

typedef struct {
    u16 breakJumps[256];
    int breakCount;
    u16 continueJumps[256];
    int continueCount;
} LoopContext;

typedef struct {
    Arena arena;
    Arena scratch;
    Source src;
    Lines lines;
    int i;
    LoopContext loops[32];
    int loopDepth;
    int functionDepth;
} LineReader;

static JattiStatus compile_block(VM* vm, LineReader* r, Chunk* chunk, int baseIndent, CompileError* err);

static JattiStatus compile_statement(VM* vm, LineReader* r, Chunk* chunk, int baseIndent, CompileError* err) {
    const char* raw = r->lines.lines[r->i];
    int lineNo = r->i + 1;

    const char* stripped = NULL;
    r->scratch.used = 0;
    if (source_strip_comments(&r->scratch, raw, &stripped) != JATTI_OK) {
        err->line = lineNo;
        err->message = "Out of memory";
        return JATTI_ERR;
    }

    if (is_blank(stripped)) {
        r->i++;
        return JATTI_OK;
    }

    int indent = indent_level(stripped);
    if (indent < baseIndent) {
        return JATTI_OK;
    }
    if (indent > baseIndent) {
        err->line = lineNo;
        err->message = "Unexpected indentation.";
        return JATTI_ERR;
    }

    const char* s = ltrim(stripped);

    if (starts_with(s, "wapas_kar")) {
        if (r->functionDepth <= 0) {
            err->line = lineNo;
            err->message = "wapas_kar can only be used inside kaam.";
            return JATTI_ERR;
        }
        s += strlen("wapas_kar");
        s = ltrim(s);

        if (*s == '\0') {
            chunk_write(chunk, OP_NIL, lineNo);
        } else {
            const char* exprErr = NULL;
            if (expr_compile(vm, chunk, s, lineNo, &exprErr) != JATTI_OK) {
                err->line = lineNo;
                err->message = exprErr;
                return JATTI_ERR;
            }
        }
        chunk_write(chunk, OP_RETURN, lineNo);
        r->i++;
        return JATTI_OK;
    }

    if (starts_with(s, "kaam")) {
        s += strlen("kaam");
        s = ltrim(s);

        const char* nameStart = s;
        while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') || *s == '_') s++;
        int nameLen = (int)(s - nameStart);
        s = ltrim(s);

        if (*s != '(') {
            err->line = lineNo;
            err->message = "Expected '(' after kaam name.";
            return JATTI_ERR;
        }
        s++;

        const char* rp = strchr(s, ')');
        if (!rp) {
            err->line = lineNo;
            err->message = "Expected ')' after kaam params.";
            return JATTI_ERR;
        }

        ObjString* fnName = table_intern_string(&vm->strings, nameStart, nameLen);
        if (!fnName) {
            err->line = lineNo;
            err->message = "Out of memory";
            return JATTI_ERR;
        }

        ObjString** params = NULL;
        int arity = 0;
        int cap = 0;
        const char* p = s;
        while (p < rp) {
            while (p < rp && (*p == ' ' || *p == '\t' || *p == ',')) p++;
            if (p >= rp) break;

            const char* ps = p;
            while (p < rp && ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_')) p++;
            int pl = (int)(p - ps);
            if (pl <= 0) {
                free(params);
                err->line = lineNo;
                err->message = "Invalid parameter name.";
                return JATTI_ERR;
            }

            if (arity >= cap) {
                int newCap = cap < 4 ? 4 : cap * 2;
                ObjString** newParams = (ObjString**)realloc(params, (size_t)newCap * sizeof(ObjString*));
                if (!newParams) {
                    free(params);
                    err->line = lineNo;
                    err->message = "Out of memory";
                    return JATTI_ERR;
                }
                params = newParams;
                cap = newCap;
            }

            ObjString* paramName = table_intern_string(&vm->strings, ps, pl);
            if (!paramName) {
                free(params);
                err->line = lineNo;
                err->message = "Out of memory";
                return JATTI_ERR;
            }
            params[arity++] = paramName;

            while (p < rp && (*p == ' ' || *p == '\t')) p++;
            if (p < rp && *p == ',') p++;
        }

        ObjFunction* fn = obj_function_new(fnName, arity, params);
        if (!fn) {
            free(params);
            err->line = lineNo;
            err->message = "Out of memory";
            return JATTI_ERR;
        }

        r->i++;
        int nextIndent = baseIndent + 1;
        r->functionDepth++;
        if (compile_block(vm, r, &fn->chunk, nextIndent, err) != JATTI_OK) {
            r->functionDepth--;
            return JATTI_ERR;
        }
        r->functionDepth--;

        chunk_write(&fn->chunk, OP_NIL, lineNo);
        chunk_write(&fn->chunk, OP_RETURN, lineNo);

        int fnConst = chunk_add_const(chunk, OBJ_VAL(fn));
        chunk_write(chunk, OP_CONST, lineNo);
        chunk_write_u16(chunk, (u16)fnConst, lineNo);

        u16 nameConst = 0;
        emit_const_name(vm, chunk, nameStart, nameLen, lineNo, &nameConst);
        emit_op_u16(chunk, OP_SET_GLOBAL, nameConst, lineNo);
        chunk_write(chunk, OP_POP, lineNo);
        return JATTI_OK;
    }

    if (starts_with(s, "chal_koshish_karle")) {
        int nextIndent = baseIndent + 1;
        u16 tryOffPos = emit_try(chunk, lineNo);

        r->i++;
        if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) return JATTI_ERR;

        chunk_write(chunk, OP_END_TRY, lineNo);
        u16 jmpEnd = emit_jump(chunk, OP_JMP, lineNo);
        patch_jump(chunk, tryOffPos);

        if (r->i >= r->lines.count) {
            err->line = lineNo;
            err->message = "Missing pakad block after chal_koshish_karle.";
            return JATTI_ERR;
        }

        const char* catchRaw = r->lines.lines[r->i];
        const char* catchStripped = NULL;
        r->scratch.used = 0;
        source_strip_comments(&r->scratch, catchRaw, &catchStripped);
        if (is_blank(catchStripped) || indent_level(catchStripped) != baseIndent) {
            err->line = r->i + 1;
            err->message = "Expected pakad block at same indentation level.";
            return JATTI_ERR;
        }

        const char* t = ltrim(catchStripped);
        if (!starts_with(t, "pakad")) {
            err->line = r->i + 1;
            err->message = "Expected pakad after chal_koshish_karle.";
            return JATTI_ERR;
        }
        t += strlen("pakad");
        t = ltrim(t);

        const char* varStart = t;
        while ((*t >= 'a' && *t <= 'z') || (*t >= 'A' && *t <= 'Z') || (*t >= '0' && *t <= '9') || *t == '_') t++;
        int varLen = (int)(t - varStart);
        if (varLen <= 0) {
            err->line = r->i + 1;
            err->message = "pakad requires an error variable name.";
            return JATTI_ERR;
        }

        emit_get_global(vm, chunk, "__jatti_err", 11, r->i + 1);
        u16 catchVarConst = 0;
        emit_const_name(vm, chunk, varStart, varLen, r->i + 1, &catchVarConst);
        emit_op_u16(chunk, OP_SET_GLOBAL, catchVarConst, r->i + 1);
        chunk_write(chunk, OP_POP, r->i + 1);

        r->i++;
        if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) return JATTI_ERR;

        patch_jump(chunk, jmpEnd);
        return JATTI_OK;
    }

    if (starts_with(s, "roko_oye_roko")) {
        if (r->loopDepth <= 0) {
            err->line = lineNo;
            err->message = "roko_oye_roko can only be used inside a loop.";
            return JATTI_ERR;
        }
        LoopContext* ctx = &r->loops[r->loopDepth - 1];
        if (ctx->breakCount >= 256) {
            err->line = lineNo;
            err->message = "Too many break statements in loop.";
            return JATTI_ERR;
        }
        ctx->breakJumps[ctx->breakCount++] = emit_jump(chunk, OP_JMP, lineNo);
        r->i++;
        return JATTI_OK;
    }

    if (starts_with(s, "chalo_oye_chalo")) {
        if (r->loopDepth <= 0) {
            err->line = lineNo;
            err->message = "chalo_oye_chalo can only be used inside a loop.";
            return JATTI_ERR;
        }
        LoopContext* ctx = &r->loops[r->loopDepth - 1];
        if (ctx->continueCount >= 256) {
            err->line = lineNo;
            err->message = "Too many continue statements in loop.";
            return JATTI_ERR;
        }
        ctx->continueJumps[ctx->continueCount++] = emit_jump(chunk, OP_JMP, lineNo);
        r->i++;
        return JATTI_OK;
    }

    // Assignment: chal_oye <name> ban <expr>
    if (starts_with(s, "chal_oye")) {
        s += strlen("chal_oye");
        s = ltrim(s);
        const char* nameStart = s;
        while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') || *s == '_') s++;
        int nameLen = (int)(s - nameStart);
        s = ltrim(s);
        if (!starts_with(s, "ban")) {
            err->line = lineNo;
            err->message = "Expected 'ban' in assignment.";
            return JATTI_ERR;
        }
        s += strlen("ban");
        s = ltrim(s);

        const char* expr = s;
        const char* exprErr = NULL;
        if (expr_compile(vm, chunk, expr, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            return JATTI_ERR;
        }

        u16 nameConst = 0;
        emit_const_name(vm, chunk, nameStart, nameLen, lineNo, &nameConst);
        emit_op_u16(chunk, OP_SET_GLOBAL, nameConst, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        r->i++;
        return JATTI_OK;
    }

    // Print: chilla_we <expr>
    if (starts_with(s, "chilla_we")) {
        s += strlen("chilla_we");
        s = ltrim(s);
        const char* expr = s;
        const char* exprErr = NULL;
        if (expr_compile(vm, chunk, expr, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            return JATTI_ERR;
        }
        chunk_write(chunk, OP_PRINT, lineNo);
        r->i++;
        return JATTI_OK;
    }

    // For loop: supports range_banao(...) and generic iterable (list/dict/string).
    if (starts_with(s, "har_ek")) {
        static int for_id = 0;
        int id = for_id++;

        s += strlen("har_ek");
        s = ltrim(s);
        const char* varStart = s;
        while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') || *s == '_') s++;
        int varLen = (int)(s - varStart);
        if (varLen <= 0) {
            err->line = lineNo;
            err->message = "har_ek requires loop variable name.";
            return JATTI_ERR;
        }

        const char* var2Start = NULL;
        int var2Len = 0;
        s = ltrim(s);

        if (*s == ',') {
            s++;
            s = ltrim(s);
            var2Start = s;
            while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') || *s == '_') s++;
            var2Len = (int)(s - var2Start);
            if (var2Len <= 0) {
                err->line = lineNo;
                err->message = "har_ek key,value requires second variable name.";
                return JATTI_ERR;
            }
            s = ltrim(s);
        }

        if (!starts_with(s, "range_banao")) {
            // Generic iterable lowering.
            char iterName[64];
            char idxName[64];
            char lenName[64];
            snprintf(iterName, sizeof(iterName), "__jatti_for_iter_%d", id);
            snprintf(idxName, sizeof(idxName), "__jatti_for_i_%d", id);
            snprintf(lenName, sizeof(lenName), "__jatti_for_len_%d", id);

            const char* iterableExpr = s;
            if (*iterableExpr == '\0') {
                err->line = lineNo;
                err->message = "har_ek requires an iterable expression.";
                return JATTI_ERR;
            }

            const char* exprErr = NULL;
            if (expr_compile(vm, chunk, iterableExpr, lineNo, &exprErr) != JATTI_OK) {
                err->line = lineNo;
                err->message = exprErr;
                return JATTI_ERR;
            }
            u16 iterConst = 0;
            emit_const_name(vm, chunk, iterName, (int)strlen(iterName), lineNo, &iterConst);
            emit_op_u16(chunk, OP_SET_GLOBAL, iterConst, lineNo);
            chunk_write(chunk, OP_POP, lineNo);

            emit_number_const(chunk, 0, lineNo);
            u16 idxConst = 0;
            emit_const_name(vm, chunk, idxName, (int)strlen(idxName), lineNo, &idxConst);
            emit_op_u16(chunk, OP_SET_GLOBAL, idxConst, lineNo);
            chunk_write(chunk, OP_POP, lineNo);

            emit_get_global(vm, chunk, iterName, (int)strlen(iterName), lineNo);
            chunk_write(chunk, OP_LEN, lineNo);
            u16 lenConst = 0;
            emit_const_name(vm, chunk, lenName, (int)strlen(lenName), lineNo, &lenConst);
            emit_op_u16(chunk, OP_SET_GLOBAL, lenConst, lineNo);
            chunk_write(chunk, OP_POP, lineNo);

            int loopStart = chunk->count;
            if (r->loopDepth >= 32) {
                err->line = lineNo;
                err->message = "Loop nesting too deep.";
                return JATTI_ERR;
            }
            LoopContext* loopCtx = &r->loops[r->loopDepth++];
            loopCtx->breakCount = 0;
            loopCtx->continueCount = 0;

            emit_get_global(vm, chunk, idxName, (int)strlen(idxName), lineNo);
            emit_get_global(vm, chunk, lenName, (int)strlen(lenName), lineNo);
            chunk_write(chunk, OP_LT, lineNo);
            u16 exitJump = emit_jump(chunk, OP_JMP_IF_FALSE, lineNo);
            chunk_write(chunk, OP_POP, lineNo);

            if (var2Start && var2Len > 0) {
                // key = iterable[idx]
                emit_get_global(vm, chunk, iterName, (int)strlen(iterName), lineNo);
                emit_get_global(vm, chunk, idxName, (int)strlen(idxName), lineNo);
                chunk_write(chunk, OP_GET_INDEX, lineNo);
                u16 keyConst = 0;
                emit_const_name(vm, chunk, varStart, varLen, lineNo, &keyConst);
                emit_op_u16(chunk, OP_SET_GLOBAL, keyConst, lineNo);
                chunk_write(chunk, OP_POP, lineNo);

                // value = iterable[key]
                emit_get_global(vm, chunk, iterName, (int)strlen(iterName), lineNo);
                emit_get_global(vm, chunk, varStart, varLen, lineNo);
                chunk_write(chunk, OP_GET_INDEX, lineNo);
                u16 valConst = 0;
                emit_const_name(vm, chunk, var2Start, var2Len, lineNo, &valConst);
                emit_op_u16(chunk, OP_SET_GLOBAL, valConst, lineNo);
                chunk_write(chunk, OP_POP, lineNo);
            } else {
                emit_get_global(vm, chunk, iterName, (int)strlen(iterName), lineNo);
                emit_get_global(vm, chunk, idxName, (int)strlen(idxName), lineNo);
                chunk_write(chunk, OP_GET_INDEX, lineNo);
                u16 varConst = 0;
                emit_const_name(vm, chunk, varStart, varLen, lineNo, &varConst);
                emit_op_u16(chunk, OP_SET_GLOBAL, varConst, lineNo);
                chunk_write(chunk, OP_POP, lineNo);
            }

            r->i++;
            int nextIndent = baseIndent + 1;
            if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) {
                r->loopDepth--;
                return JATTI_ERR;
            }

            int continueTarget = chunk->count;
            for (int k = 0; k < loopCtx->continueCount; k++) {
                patch_jump_to(chunk, loopCtx->continueJumps[k], continueTarget);
            }

            emit_get_global(vm, chunk, idxName, (int)strlen(idxName), lineNo);
            emit_number_const(chunk, 1, lineNo);
            chunk_write(chunk, OP_ADD, lineNo);
            emit_op_u16(chunk, OP_SET_GLOBAL, idxConst, lineNo);
            chunk_write(chunk, OP_POP, lineNo);

            chunk_write(chunk, OP_LOOP, lineNo);
            u16 backOff = (u16)(chunk->count - loopStart + 2);
            chunk_write_u16(chunk, backOff, lineNo);

            patch_jump(chunk, exitJump);
            chunk_write(chunk, OP_POP, lineNo);

            int breakTarget = chunk->count;
            for (int k = 0; k < loopCtx->breakCount; k++) {
                patch_jump_to(chunk, loopCtx->breakJumps[k], breakTarget);
            }
            r->loopDepth--;
            return JATTI_OK;
        }

        if (var2Start && var2Len > 0) {
            err->line = lineNo;
            err->message = "har_ek key,value is not supported with range_banao(...).";
            return JATTI_ERR;
        }
        const char* lp = strchr(s, '(');
        const char* rp = strrchr(s, ')');
        if (!lp || !rp || rp < lp) {
            err->line = lineNo;
            err->message = "range_banao(...) syntax expected.";
            return JATTI_ERR;
        }

        // Extract args
        const char* args = lp + 1;
        size_t argsLen = (size_t)(rp - args);
        char* argsCopy = arena_strdup(&r->arena, args, argsLen);
        if (!argsCopy) {
            err->line = lineNo;
            err->message = "Out of memory";
            return JATTI_ERR;
        }

        // Find comma at top-level (simple; good enough for MVP)
        bool in_single = false, in_double = false, escape = false;
        int depth = 0;
        char* comma = NULL;
        for (char* p = argsCopy; *p; p++) {
            char c = *p;
            if (escape) { escape = false; continue; }
            if ((in_single || in_double) && c == '\\') { escape = true; continue; }
            if (!in_single && c == '"') { in_double = !in_double; continue; }
            if (!in_double && c == '\'') { in_single = !in_single; continue; }
            if (in_single || in_double) continue;
            if (c == '(' || c == '[' || c == '{') depth++;
            else if (c == ')' || c == ']' || c == '}') depth = depth > 0 ? depth - 1 : 0;
            else if (c == ',' && depth == 0) { comma = p; break; }
        }

        char* a1 = argsCopy;
        char* a2 = NULL;
        if (comma) {
            *comma = '\0';
            a2 = comma + 1;
        }

        // trim
        while (*a1 == ' ' || *a1 == '\t') a1++;
        if (a2) { while (*a2 == ' ' || *a2 == '\t') a2++; }
        char* end1 = a1 + strlen(a1);
        while (end1 > a1 && (end1[-1] == ' ' || end1[-1] == '\t')) *--end1 = '\0';
        if (a2) {
            char* end2 = a2 + strlen(a2);
            while (end2 > a2 && (end2[-1] == ' ' || end2[-1] == '\t')) *--end2 = '\0';
        }

        const char* startExpr = NULL;
        const char* endExpr = NULL;
        if (!a2 || *a2 == '\0') {
            startExpr = "0";
            endExpr = a1;
        } else {
            startExpr = a1;
            endExpr = a2;
        }

        char endName[64];
        snprintf(endName, sizeof(endName), "__jatti_for_end_%d", id);

        // i = start
        const char* exprErr = NULL;
        if (expr_compile(vm, chunk, startExpr, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            return JATTI_ERR;
        }
        u16 varConst = 0;
        emit_const_name(vm, chunk, varStart, varLen, lineNo, &varConst);
        emit_op_u16(chunk, OP_SET_GLOBAL, varConst, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        // endVar = end
        if (expr_compile(vm, chunk, endExpr, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            return JATTI_ERR;
        }
        u16 endConst = 0;
        emit_const_name(vm, chunk, endName, (int)strlen(endName), lineNo, &endConst);
        emit_op_u16(chunk, OP_SET_GLOBAL, endConst, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        int loopStart = chunk->count;

        if (r->loopDepth >= 32) {
            err->line = lineNo;
            err->message = "Loop nesting too deep.";
            return JATTI_ERR;
        }
        LoopContext* loopCtx = &r->loops[r->loopDepth++];
        loopCtx->breakCount = 0;
        loopCtx->continueCount = 0;

        // condition: i < end
        emit_get_global(vm, chunk, varStart, varLen, lineNo);
        emit_get_global(vm, chunk, endName, (int)strlen(endName), lineNo);
        chunk_write(chunk, OP_LT, lineNo);
        u16 exitJump = emit_jump(chunk, OP_JMP_IF_FALSE, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        r->i++;
        int nextIndent = baseIndent + 1;
        if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) {
            r->loopDepth--;
            return JATTI_ERR;
        }

        int continueTarget = chunk->count;
        for (int k = 0; k < loopCtx->continueCount; k++) {
            patch_jump_to(chunk, loopCtx->continueJumps[k], continueTarget);
        }

        // increment i = i + 1
        emit_get_global(vm, chunk, varStart, varLen, lineNo);
        emit_number_const(chunk, 1, lineNo);
        chunk_write(chunk, OP_ADD, lineNo);
        emit_op_u16(chunk, OP_SET_GLOBAL, varConst, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        // loop
        chunk_write(chunk, OP_LOOP, lineNo);
        u16 backOff = (u16)(chunk->count - loopStart + 2);
        chunk_write_u16(chunk, backOff, lineNo);

        patch_jump(chunk, exitJump);
        chunk_write(chunk, OP_POP, lineNo);

        int breakTarget = chunk->count;
        for (int k = 0; k < loopCtx->breakCount; k++) {
            patch_jump_to(chunk, loopCtx->breakJumps[k], breakTarget);
        }
        r->loopDepth--;
        return JATTI_OK;
    }

    // If: je <expr>
    if (starts_with(s, "je")) {
        s += strlen("je");
        s = ltrim(s);
        const char* exprErr = NULL;
        if (expr_compile(vm, chunk, s, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            return JATTI_ERR;
        }
        u16 jmpFalsePos = emit_jump(chunk, OP_JMP_IF_FALSE, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        r->i++;
        // then block must be next indent
        int nextIndent = baseIndent + 1;
        if (r->i >= r->lines.count) {
            err->line = lineNo;
            err->message = "Missing if body.";
            return JATTI_ERR;
        }
        if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) return JATTI_ERR;

        u16 endJumps[128];
        int endCount = 0;
        endJumps[endCount++] = emit_jump(chunk, OP_JMP, lineNo);
        patch_jump(chunk, jmpFalsePos);
        chunk_write(chunk, OP_POP, lineNo);

        // optional nahin_taan_je chain
        while (r->i < r->lines.count) {
            const char* raw2 = r->lines.lines[r->i];
            const char* stripped2 = NULL;
            r->scratch.used = 0;
            source_strip_comments(&r->scratch, raw2, &stripped2);
            if (is_blank(stripped2) || indent_level(stripped2) != baseIndent) break;

            const char* t = ltrim(stripped2);
            if (!starts_with(t, "nahin_taan_je")) break;

            t += strlen("nahin_taan_je");
            t = ltrim(t);
            if (expr_compile(vm, chunk, t, r->i + 1, &exprErr) != JATTI_OK) {
                err->line = r->i + 1;
                err->message = exprErr;
                return JATTI_ERR;
            }

            u16 elifFalse = emit_jump(chunk, OP_JMP_IF_FALSE, r->i + 1);
            chunk_write(chunk, OP_POP, r->i + 1);

            r->i++;
            if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) return JATTI_ERR;

            if (endCount >= 128) {
                err->line = r->i + 1;
                err->message = "Too many else-if branches.";
                return JATTI_ERR;
            }
            endJumps[endCount++] = emit_jump(chunk, OP_JMP, r->i + 1);
            patch_jump(chunk, elifFalse);
            chunk_write(chunk, OP_POP, r->i + 1);
        }

        // optional final else
        if (r->i < r->lines.count) {
            const char* raw2 = r->lines.lines[r->i];
            const char* stripped2 = NULL;
            r->scratch.used = 0;
            source_strip_comments(&r->scratch, raw2, &stripped2);
            if (!is_blank(stripped2) && indent_level(stripped2) == baseIndent) {
                const char* t = ltrim(stripped2);
                if (starts_with(t, "nahin_taan")) {
                    r->i++;
                    if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) return JATTI_ERR;
                }
            }
        }

        for (int k = 0; k < endCount; k++) {
            patch_jump(chunk, endJumps[k]);
        }
        return JATTI_OK;
    }

    // While: jadon_tak <expr>
    if (starts_with(s, "jadon_tak")) {
        int loopStart = chunk->count;

        if (r->loopDepth >= 32) {
            err->line = lineNo;
            err->message = "Loop nesting too deep.";
            return JATTI_ERR;
        }
        LoopContext* loopCtx = &r->loops[r->loopDepth++];
        loopCtx->breakCount = 0;
        loopCtx->continueCount = 0;

        s += strlen("jadon_tak");
        s = ltrim(s);
        const char* exprErr = NULL;
        if (expr_compile(vm, chunk, s, lineNo, &exprErr) != JATTI_OK) {
            err->line = lineNo;
            err->message = exprErr;
            r->loopDepth--;
            return JATTI_ERR;
        }
        u16 exitJump = emit_jump(chunk, OP_JMP_IF_FALSE, lineNo);
        chunk_write(chunk, OP_POP, lineNo);

        r->i++;
        int nextIndent = baseIndent + 1;
        if (compile_block(vm, r, chunk, nextIndent, err) != JATTI_OK) {
            r->loopDepth--;
            return JATTI_ERR;
        }

        int continueTarget = chunk->count;
        for (int k = 0; k < loopCtx->continueCount; k++) {
            patch_jump_to(chunk, loopCtx->continueJumps[k], continueTarget);
        }

        // jump back
        // OP_LOOP expects a positive offset to subtract from ip.
        chunk_write(chunk, OP_LOOP, lineNo);
        u16 backOff = (u16)(chunk->count - loopStart + 2);
        chunk_write_u16(chunk, backOff, lineNo);

        patch_jump(chunk, exitJump);
        chunk_write(chunk, OP_POP, lineNo);

        int breakTarget = chunk->count;
        for (int k = 0; k < loopCtx->breakCount; k++) {
            patch_jump_to(chunk, loopCtx->breakJumps[k], breakTarget);
        }
        r->loopDepth--;
        return JATTI_OK;
    }

    err->line = lineNo;
    err->message = "Unsupported statement (MVP compiler).";
    return JATTI_ERR;
}

static JattiStatus compile_block(VM* vm, LineReader* r, Chunk* chunk, int baseIndent, CompileError* err) {
    while (r->i < r->lines.count) {
        const char* raw = r->lines.lines[r->i];
        const char* stripped = NULL;
        r->scratch.used = 0;
        source_strip_comments(&r->scratch, raw, &stripped);
        if (is_blank(stripped)) { r->i++; continue; }
        int indent = indent_level(stripped);
        if (indent < baseIndent) break;
        if (indent > baseIndent) {
            err->line = r->i + 1;
            err->message = "Unexpected indentation.";
            return JATTI_ERR;
        }
        if (compile_statement(vm, r, chunk, baseIndent, err) != JATTI_OK) return JATTI_ERR;
    }
    return JATTI_OK;
}

JattiStatus jatti_compile_file(VM* vm, const char* path, Chunk* out_chunk, CompileError* out_err) {
    Arena arena;
    arena_init(&arena);

    LineReader r;
    r.arena = arena;
    arena_init(&r.scratch);
    if (source_read_file(&r.arena, path, &r.src) != JATTI_OK) {
        out_err->line = 0;
        out_err->message = "Could not read file.";
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }
    if (source_split_lines(&r.arena, r.src, &r.lines) != JATTI_OK) {
        out_err->line = 0;
        out_err->message = "Out of memory.";
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }

    // Validate sun_we/ja_we structure (MVP).
    int first = -1;
    int last = -1;
    for (int i = 0; i < r.lines.count; i++) {
        const char* stripped = NULL;
        r.scratch.used = 0;
        source_strip_comments(&r.scratch, r.lines.lines[i], &stripped);
        if (is_blank(stripped)) continue;
        if (first == -1) first = i;
        last = i;
    }
    if (first == -1) {
        out_err->line = 0;
        out_err->message = "Empty file.";
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }

    const char* firstLine = ltrim(r.lines.lines[first]);
    const char* lastLine = ltrim(r.lines.lines[last]);
    if (!starts_with(firstLine, "sun_we")) {
        out_err->line = first + 1;
        out_err->message = "Program must start with sun_we.";
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }
    if (!starts_with(lastLine, "ja_we")) {
        out_err->line = last + 1;
        out_err->message = "Program must end with ja_we.";
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }

    chunk_init(out_chunk);

    r.i = first + 1;
    r.loopDepth = 0;
    r.functionDepth = 0;
    if (compile_block(vm, &r, out_chunk, 1, out_err) != JATTI_OK) {
        arena_free(&r.arena);
        arena_free(&r.scratch);
        return JATTI_ERR;
    }

    chunk_write(out_chunk, OP_HALT, last + 1);

    arena_free(&r.arena);
    arena_free(&r.scratch);
    return JATTI_OK;
}
