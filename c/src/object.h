#pragma once

#include "common.h"
#include "value.h"
#include "chunk.h"

struct VM;
typedef JattiStatus (*NativeFn)(struct VM* vm, int argCount, Value* args, Value* out, const char** err);

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_LIST,
    OBJ_DICT,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    u32 hash;
};

typedef struct {
    Obj obj;
    int arity;
    ObjString* name;
    ObjString** params;
    Chunk chunk;
} ObjFunction;

typedef struct {
    Obj obj;
    int arity;
    ObjString* name;
    NativeFn function;
} ObjNative;

typedef struct {
    Obj obj;
    int count;
    int capacity;
    Value* items;
} ObjList;

typedef struct {
    Obj obj;
    int count;
    int capacity;
    ObjString** keys;
    Value* values;
} ObjDict;

ObjString* obj_string_copy(const char* chars, int length);
ObjFunction* obj_function_new(ObjString* name, int arity, ObjString** params);
ObjNative* obj_native_new(ObjString* name, int arity, NativeFn function);
ObjList* obj_list_new(void);
bool obj_list_push(ObjList* list, Value v);
ObjDict* obj_dict_new(void);
bool obj_dict_set(ObjDict* dict, ObjString* key, Value value);
bool obj_dict_get(ObjDict* dict, ObjString* key, Value* out);

bool obj_equals(Obj* a, Obj* b);
void obj_print(Obj* obj);

u32 hash_string(const char* key, int length);

// VM owns object list.
void obj_set_vm_object_list(Obj** head);
