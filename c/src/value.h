#pragma once

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_OBJ,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
    } as;
} Value;

// MSVC-friendly constructors (avoid designated initializers / compound literals).
static inline Value value_nil(void) {
    Value v;
    v.type = VAL_NIL;
    return v;
}

static inline Value value_bool(bool b) {
    Value v;
    v.type = VAL_BOOL;
    v.as.boolean = b;
    return v;
}

static inline Value value_number(double n) {
    Value v;
    v.type = VAL_NUMBER;
    v.as.number = n;
    return v;
}

static inline Value value_obj(void* o) {
    Value v;
    v.type = VAL_OBJ;
    v.as.obj = (Obj*)o;
    return v;
}

#define NIL_VAL() (value_nil())
#define BOOL_VAL(b) (value_bool((b)))
#define NUMBER_VAL(n) (value_number((n)))
#define OBJ_VAL(o) (value_obj((o)))

#define IS_NIL(v) ((v).type == VAL_NIL)
#define IS_BOOL(v) ((v).type == VAL_BOOL)
#define IS_NUMBER(v) ((v).type == VAL_NUMBER)
#define IS_OBJ(v) ((v).type == VAL_OBJ)

#define AS_BOOL(v) ((v).as.boolean)
#define AS_NUMBER(v) ((v).as.number)
#define AS_OBJ(v) ((v).as.obj)

bool value_is_falsey(Value v);
bool value_equals(Value a, Value b);

void value_print(Value v);
