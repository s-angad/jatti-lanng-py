#include "expr_scan.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static void skip_ws(ExprScanner* sc) {
    for (;;) {
        char c = *sc->cur;
        if (c == ' ' || c == '\t' || c == '\r') {
            sc->cur++;
            continue;
        }
        return;
    }
}

static Token make_token(ExprScanner* sc, TokenType type, const char* start) {
    Token t;
    t.type = type;
    t.start = start;
    t.length = (int)(sc->cur - start);
    t.line = sc->line;
    t.number = 0.0;
    return t;
}

static Token error_token(ExprScanner* sc, const char* msg) {
    Token t;
    t.type = TOK_ERROR;
    t.start = msg;
    t.length = (int)strlen(msg);
    t.line = sc->line;
    t.number = 0.0;
    return t;
}

void expr_scanner_init(ExprScanner* sc, const char* src, int line) {
    sc->src = src;
    sc->line = line;
    sc->cur = src;
}

static Token scan_string(ExprScanner* sc, char quote) {
    const char* start = sc->cur - 1; // includes opening quote
    bool escape = false;
    while (*sc->cur != '\0') {
        char c = *sc->cur++;
        if (escape) {
            escape = false;
            continue;
        }
        if (c == '\\') {
            escape = true;
            continue;
        }
        if (c == quote) {
            return make_token(sc, TOK_STRING, start);
        }
    }
    return error_token(sc, "Unterminated string.");
}

static Token scan_number(ExprScanner* sc, const char* start) {
    while (is_digit(*sc->cur)) sc->cur++;
    if (*sc->cur == '.' && is_digit(sc->cur[1])) {
        sc->cur++;
        while (is_digit(*sc->cur)) sc->cur++;
    }

    Token t = make_token(sc, TOK_NUMBER, start);
    // crude atof
    char tmp[64];
    int len = t.length;
    if (len >= (int)sizeof(tmp)) len = (int)sizeof(tmp) - 1;
    memcpy(tmp, start, (size_t)len);
    tmp[len] = '\0';
    t.number = strtod(tmp, NULL);
    return t;
}

static Token scan_ident_or_kw(ExprScanner* sc, const char* start) {
    while (is_alpha(*sc->cur) || is_digit(*sc->cur)) sc->cur++;
    Token t = make_token(sc, TOK_IDENT, start);

    // Keywords in expressions.
    // Logical
    if (t.length == 3 && strncmp(t.start, "ate", 3) == 0) t.type = TOK_AND;
    else if (t.length == 2 && strncmp(t.start, "te", 2) == 0) t.type = TOK_PLUS;
    else if (t.length == 3 && strncmp(t.start, "hor", 3) == 0) t.type = TOK_OR;
    else if (t.length == 5 && strncmp(t.start, "ya_te", 5) == 0) t.type = TOK_OR;
    else if (t.length == 4 && strncmp(t.start, "nahi", 4) == 0) t.type = TOK_NOT;

    // Comparison keywords
    else if (t.length == 9 && strncmp(t.start, "vadha_hai", 9) == 0) t.type = TOK_GT;
    else if (t.length == 9 && strncmp(t.start, "nikka_hai", 9) == 0) t.type = TOK_LT;
    else if (t.length == 7 && strncmp(t.start, "barabar", 7) == 0) t.type = TOK_EQEQ;
    else if (t.length == 16 && strncmp(t.start, "barabar_nahi_hai", 16) == 0) t.type = TOK_NEQ;
    else if (t.length == 16 && strncmp(t.start, "vadha_ya_barabar", 16) == 0) t.type = TOK_GTE;
    else if (t.length == 16 && strncmp(t.start, "nikka_ya_barabar", 16) == 0) t.type = TOK_LTE;
    else if (t.length == 11 && strncmp(t.start, "kinna_lamba", 11) == 0) t.type = TOK_LEN;

    return t;
}

Token expr_scan_next(ExprScanner* sc) {
    skip_ws(sc);

    const char* start = sc->cur;
    char c = *sc->cur++;

    if (c == '\0') return make_token(sc, TOK_EOF, start);

    if (is_digit(c)) return scan_number(sc, start);
    if (is_alpha(c)) return scan_ident_or_kw(sc, start);

    switch (c) {
        case '(': return make_token(sc, TOK_LPAREN, start);
        case ')': return make_token(sc, TOK_RPAREN, start);
        case ',': return make_token(sc, TOK_COMMA, start);
        case '[': return make_token(sc, TOK_LBRACKET, start);
        case ']': return make_token(sc, TOK_RBRACKET, start);
        case '{': return make_token(sc, TOK_LBRACE, start);
        case '}': return make_token(sc, TOK_RBRACE, start);
        case ':': return make_token(sc, TOK_COLON, start);
        case '+': return make_token(sc, TOK_PLUS, start);
        case '-': return make_token(sc, TOK_MINUS, start);
        case '*':
            if (*sc->cur == '*') { sc->cur++; return make_token(sc, TOK_STARSTAR, start); }
            return make_token(sc, TOK_STAR, start);
        case '/': return make_token(sc, TOK_SLASH, start);
        case '%': return make_token(sc, TOK_PERCENT, start);
        case '"': return scan_string(sc, '"');
        case '\'': return scan_string(sc, '\'');
        case '=':
            if (*sc->cur == '=') { sc->cur++; return make_token(sc, TOK_EQEQ, start); }
            break;
        case '!':
            if (*sc->cur == '=') { sc->cur++; return make_token(sc, TOK_NEQ, start); }
            break;
        case '<':
            if (*sc->cur == '=') { sc->cur++; return make_token(sc, TOK_LTE, start); }
            return make_token(sc, TOK_LT, start);
        case '>':
            if (*sc->cur == '=') { sc->cur++; return make_token(sc, TOK_GTE, start); }
            return make_token(sc, TOK_GT, start);
    }

    return error_token(sc, "Unexpected character.");
}
