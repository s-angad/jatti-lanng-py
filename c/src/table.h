#pragma once

#include "common.h"
#include "value.h"
#include "object.h"

typedef struct {
    ObjString* key;
    Value value;
    bool occupied;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void table_init(Table* table);
void table_free(Table* table);

bool table_get(Table* table, ObjString* key, Value* out);
bool table_set(Table* table, ObjString* key, Value value);

ObjString* table_intern_string(Table* table, const char* chars, int length);
