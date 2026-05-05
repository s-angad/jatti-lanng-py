#include "expr_parse.h"

#include <string.h>

#include "object.h"
#include "table.h"

typedef struct {
    VM* vm;
    Chunk* chunk;
    ExprScanner sc;
    Token current;
    Token previous;
    const char* err;
} ExprParser;

static void advance(ExprParser* p) {
    p->previous = p->current;
    for (;;) {
        p->current = expr_scan_next(&p->sc);
        if (p->current.type != TOK_ERROR) break;
        p->err = p->current.start;
        break;
    }
}

static bool match(ExprParser* p, TokenType type) {
    if (p->current.type != type) return false;
    advance(p);
    return true;
}

static void emit(ExprParser* p, u8 byte, int line) {
    chunk_write(p->chunk, byte, line);
}

static void emit_u16(ExprParser* p, u16 v, int line) {
    chunk_write_u16(p->chunk, v, line);
}

static void emit_const(ExprParser* p, Value v, int line) {
    int idx = chunk_add_const(p->chunk, v);
    emit(p, OP_CONST, line);
    emit_u16(p, (u16)idx, line);
}

static void emit_op_u16(ExprParser* p, u8 op, u16 operand, int line) {
    emit(p, op, line);
    emit_u16(p, operand, line);
}

static u16 emit_jump(ExprParser* p, u8 op, int line) {
    emit(p, op, line);
    emit_u16(p, 0xFFFF, line);
    return (u16)(p->chunk->count - 2);
}

static void patch_jump(ExprParser* p, u16 offsetPos) {
    int jumpFrom = (int)offsetPos + 2;
    int jumpTo = p->chunk->count;
    int off = jumpTo - jumpFrom;
    if (off < 0) off = 0;
    u16 uoff = (u16)off;
    p->chunk->code[offsetPos] = (u8)((uoff >> 8) & 0xFF);
    p->chunk->code[offsetPos + 1] = (u8)(uoff & 0xFF);
}

static void expression(ExprParser* p);

// Pratt parser

typedef enum {
    PREC_NONE,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_POWER,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY,
} Precedence;

typedef void (*ParseFn)(ExprParser* p);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence prec;
} Rule;

static void parse_precedence(ExprParser* p, Precedence prec);

static void number(ExprParser* p) {
    emit_const(p, NUMBER_VAL(p->previous.number), p->previous.line);
}

static void string(ExprParser* p) {
    // token includes quotes
    const char* s = p->previous.start;
    int len = p->previous.length;
    if (len < 2) { p->err = "Bad string"; return; }
    char quote = s[0];
    JATTI_UNUSED(quote);

    // Copy raw inner chars; for MVP we do not unescape sequences.
    const char* inner = s + 1;
    int innerLen = len - 2;

    ObjString* obj = obj_string_copy(inner, innerLen);
    if (!obj) { p->err = "Out of memory"; return; }
    emit_const(p, OBJ_VAL(obj), p->previous.line);
}

static void ident(ExprParser* p) {
    // Handle literals: sach, jhoot, khaali
    if (p->previous.length == 4 && strncmp(p->previous.start, "sach", 4) == 0) {
        emit(p, OP_TRUE, p->previous.line);
        return;
    }
    if (p->previous.length == 5 && strncmp(p->previous.start, "jhoot", 5) == 0) {
        emit(p, OP_FALSE, p->previous.line);
        return;
    }
    if (p->previous.length == 6 && strncmp(p->previous.start, "khaali", 6) == 0) {
        emit(p, OP_NIL, p->previous.line);
        return;
    }

    ObjString* name = table_intern_string(&p->vm->strings, p->previous.start, p->previous.length);
    if (!name) { p->err = "Out of memory"; return; }
    int idx = chunk_add_const(p->chunk, OBJ_VAL(name));
    emit_op_u16(p, OP_GET_GLOBAL, (u16)idx, p->previous.line);
}

static void grouping(ExprParser* p) {
    expression(p);
    if (!match(p, TOK_RPAREN)) {
        p->err = "Expected ')'";
    }
}

static void len_builtin(ExprParser* p) {
    int line = p->previous.line;
    if (!match(p, TOK_LPAREN)) {
        p->err = "Expected '(' after kinna_lamba";
        return;
    }
    expression(p);
    if (p->err) return;
    if (!match(p, TOK_RPAREN)) {
        p->err = "Expected ')' after kinna_lamba argument.";
        return;
    }
    emit(p, OP_LEN, line);
}

static void list_literal(ExprParser* p) {
    int itemCount = 0;
    int line = p->previous.line;

    if (!match(p, TOK_RBRACKET)) {
        do {
            expression(p);
            if (p->err) return;
            itemCount++;
            if (itemCount > 255) {
                p->err = "Too many list elements.";
                return;
            }
        } while (match(p, TOK_COMMA));

        if (!match(p, TOK_RBRACKET)) {
            p->err = "Expected ']' after list literal.";
            return;
        }
    }

    emit(p, OP_LIST, line);
    emit(p, (u8)itemCount, line);
}

static void dict_literal(ExprParser* p) {
    int pairCount = 0;
    int line = p->previous.line;

    if (!match(p, TOK_RBRACE)) {
        do {
            if (p->current.type != TOK_STRING) {
                p->err = "Dictionary keys must be string literals.";
                return;
            }

            advance(p);
            string(p);
            if (p->err) return;

            if (!match(p, TOK_COLON)) {
                p->err = "Expected ':' after dictionary key.";
                return;
            }

            expression(p);
            if (p->err) return;

            pairCount++;
            if (pairCount > 255) {
                p->err = "Too many dictionary entries.";
                return;
            }
        } while (match(p, TOK_COMMA));

        if (!match(p, TOK_RBRACE)) {
            p->err = "Expected '}' after dictionary literal.";
            return;
        }
    }

    emit(p, OP_DICT, line);
    emit(p, (u8)pairCount, line);
}

static void index_get(ExprParser* p) {
    int line = p->previous.line;
    expression(p);
    if (p->err) return;
    if (!match(p, TOK_RBRACKET)) {
        p->err = "Expected ']' after index expression.";
        return;
    }
    emit(p, OP_GET_INDEX, line);
}

static void call(ExprParser* p) {
    int argCount = 0;
    if (!match(p, TOK_RPAREN)) {
        do {
            expression(p);
            if (p->err) return;
            argCount++;
            if (argCount > 255) {
                p->err = "Too many function arguments.";
                return;
            }
        } while (match(p, TOK_COMMA));

        if (!match(p, TOK_RPAREN)) {
            p->err = "Expected ')' after arguments.";
            return;
        }
    }

    emit(p, OP_CALL, p->previous.line);
    emit(p, (u8)argCount, p->previous.line);
}

static void unary(ExprParser* p) {
    TokenType op = p->previous.type;
    parse_precedence(p, PREC_UNARY);
    switch (op) {
        case TOK_MINUS: emit(p, OP_NEG, p->previous.line); break;
        case TOK_NOT: emit(p, OP_NOT, p->previous.line); break;
        default: break;
    }
}

static void binary(ExprParser* p) {
    TokenType op = p->previous.type;
    int opLine = p->previous.line;
    Rule rule;
    // rule lookup for precedence
    // This function is called after consuming operator; previous is operator.
    // Parse right operand at one higher precedence.

    // Determine current op precedence.
    Precedence opPrec = PREC_NONE;
    switch (op) {
        case TOK_OR: opPrec = PREC_OR; break;
        case TOK_AND: opPrec = PREC_AND; break;
        case TOK_EQEQ:
        case TOK_NEQ: opPrec = PREC_EQUALITY; break;
        case TOK_LT:
        case TOK_LTE:
        case TOK_GT:
        case TOK_GTE: opPrec = PREC_COMPARISON; break;
        case TOK_PLUS:
        case TOK_MINUS: opPrec = PREC_TERM; break;
        case TOK_STAR:
        case TOK_SLASH:
        case TOK_PERCENT: opPrec = PREC_FACTOR; break;
        case TOK_STARSTAR: opPrec = PREC_POWER; break;
        default: opPrec = PREC_NONE; break;
    }
    JATTI_UNUSED(rule);

    // Short-circuit boolean operators need special handling.
    if (op == TOK_AND) {
        u16 endJump = emit_jump(p, OP_JMP_IF_FALSE, opLine);
        emit(p, OP_POP, opLine);
        parse_precedence(p, (Precedence)(PREC_AND + 1));
        patch_jump(p, endJump);
        return;
    }
    if (op == TOK_OR) {
        u16 elseJump = emit_jump(p, OP_JMP_IF_FALSE, opLine);
        u16 endJump = emit_jump(p, OP_JMP, opLine);
        patch_jump(p, elseJump);
        emit(p, OP_POP, opLine);
        parse_precedence(p, (Precedence)(PREC_OR + 1));
        patch_jump(p, endJump);
        return;
    }

    parse_precedence(p, (Precedence)(opPrec + 1));

    switch (op) {
        case TOK_PLUS: emit(p, OP_ADD, opLine); break;
        case TOK_MINUS: emit(p, OP_SUB, opLine); break;
        case TOK_STAR: emit(p, OP_MUL, opLine); break;
        case TOK_SLASH: emit(p, OP_DIV, opLine); break;
        case TOK_PERCENT: emit(p, OP_MOD, opLine); break;
        case TOK_STARSTAR: emit(p, OP_POW, opLine); break;

        case TOK_EQEQ: emit(p, OP_EQ, opLine); break;
        case TOK_NEQ: emit(p, OP_NEQ, opLine); break;
        case TOK_LT: emit(p, OP_LT, opLine); break;
        case TOK_LTE: emit(p, OP_LTE, opLine); break;
        case TOK_GT: emit(p, OP_GT, opLine); break;
        case TOK_GTE: emit(p, OP_GTE, opLine); break;

        default:
            break;
    }
}

static Rule rules[] = {
    [TOK_EOF] = {NULL, NULL, PREC_NONE},
    [TOK_ERROR] = {NULL, NULL, PREC_NONE},
    [TOK_NUMBER] = {number, NULL, PREC_NONE},
    [TOK_STRING] = {string, NULL, PREC_NONE},
    [TOK_IDENT] = {ident, NULL, PREC_NONE},
    [TOK_LEN] = {len_builtin, NULL, PREC_NONE},
    [TOK_LPAREN] = {grouping, call, PREC_CALL},
    [TOK_RPAREN] = {NULL, NULL, PREC_NONE},
    [TOK_COMMA] = {NULL, NULL, PREC_NONE},
    [TOK_LBRACKET] = {list_literal, index_get, PREC_CALL},
    [TOK_RBRACKET] = {NULL, NULL, PREC_NONE},
    [TOK_LBRACE] = {dict_literal, NULL, PREC_NONE},
    [TOK_RBRACE] = {NULL, NULL, PREC_NONE},
    [TOK_COLON] = {NULL, NULL, PREC_NONE},

    [TOK_PLUS] = {NULL, binary, PREC_TERM},
    [TOK_MINUS] = {unary, binary, PREC_TERM},
    [TOK_STAR] = {NULL, binary, PREC_FACTOR},
    [TOK_STARSTAR] = {NULL, binary, PREC_POWER},
    [TOK_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOK_PERCENT] = {NULL, binary, PREC_FACTOR},

    [TOK_EQEQ] = {NULL, binary, PREC_EQUALITY},
    [TOK_NEQ] = {NULL, binary, PREC_EQUALITY},
    [TOK_LT] = {NULL, binary, PREC_COMPARISON},
    [TOK_LTE] = {NULL, binary, PREC_COMPARISON},
    [TOK_GT] = {NULL, binary, PREC_COMPARISON},
    [TOK_GTE] = {NULL, binary, PREC_COMPARISON},

    [TOK_AND] = {NULL, binary, PREC_AND},
    [TOK_OR] = {NULL, binary, PREC_OR},
    [TOK_NOT] = {unary, NULL, PREC_NONE},
};

static Rule* get_rule(TokenType type) {
    return &rules[type];
}

static void parse_precedence(ExprParser* p, Precedence prec) {
    advance(p);
    ParseFn prefix = get_rule(p->previous.type)->prefix;
    if (!prefix) {
        p->err = "Expected expression.";
        return;
    }
    prefix(p);

    while (prec <= get_rule(p->current.type)->prec) {
        advance(p);
        ParseFn infix = get_rule(p->previous.type)->infix;
        if (infix) infix(p);
    }
}

static void expression(ExprParser* p) {
    parse_precedence(p, PREC_OR);
}

JattiStatus expr_compile(VM* vm, Chunk* chunk, const char* expr_src, int line, const char** err_msg) {
    ExprParser p;
    p.vm = vm;
    p.chunk = chunk;
    expr_scanner_init(&p.sc, expr_src, line);
    p.err = NULL;

    p.current.type = TOK_ERROR;
    p.current.start = "";
    p.current.length = 0;
    p.current.line = line;
    p.current.number = 0.0;
    p.previous = p.current;

    advance(&p);
    expression(&p);

    if (p.err) {
        *err_msg = p.err;
        return JATTI_ERR;
    }

    if (p.current.type != TOK_EOF) {
        *err_msg = "Unexpected token after expression.";
        return JATTI_ERR;
    }

    *err_msg = NULL;
    return JATTI_OK;
}
