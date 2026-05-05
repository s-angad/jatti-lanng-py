#pragma once

#include "common.h"

typedef enum {
    TOK_EOF,
    TOK_ERROR,

    TOK_NUMBER,
    TOK_STRING,
    TOK_IDENT,
    TOK_LEN,

    TOK_LPAREN,
    TOK_RPAREN,
    TOK_COMMA,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_COLON,

    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_STARSTAR,
    TOK_SLASH,
    TOK_PERCENT,

    TOK_EQEQ,
    TOK_NEQ,
    TOK_LT,
    TOK_LTE,
    TOK_GT,
    TOK_GTE,

    TOK_AND,
    TOK_OR,
    TOK_NOT,
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
    double number;
} Token;

typedef struct {
    const char* src;
    int line;
    const char* cur;
} ExprScanner;

void expr_scanner_init(ExprScanner* sc, const char* src, int line);
Token expr_scan_next(ExprScanner* sc);
