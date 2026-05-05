#include "table.h"

#include <stdlib.h>
#include <string.h>

static Entry* find_entry(Entry* entries, int capacity, ObjString* key) {
    u32 index = key->hash & (u32)(capacity - 1);
    for (;;) {
        Entry* e = &entries[index];
        if (!e->occupied) {
            return e;
        }
        if (e->key && e->key->hash == key->hash && e->key->length == key->length &&
            memcmp(e->key->chars, key->chars, (size_t)key->length) == 0) {
            return e;
        }
        index = (index + 1) & (u32)(capacity - 1);
    }
}

static void adjust_capacity(Table* table, int capacity) {
    Entry* entries = (Entry*)calloc((size_t)capacity, sizeof(Entry));
    for (int i = 0; i < capacity; i++) {
        entries[i].occupied = false;
        entries[i].key = NULL;
        entries[i].value = NIL_VAL();
    }

    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        Entry* old = &table->entries[i];
        if (!old->occupied || !old->key) continue;
        Entry* dest = find_entry(entries, capacity, old->key);
        dest->occupied = true;
        dest->key = old->key;
        dest->value = old->value;
        table->count++;
    }

    free(table->entries);
    table->entries = entries;
    table->capacity = capacity;
}

void table_init(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void table_free(Table* table) {
    free(table->entries);
    table_init(table);
}

bool table_get(Table* table, ObjString* key, Value* out) {
    if (table->capacity == 0) return false;
    Entry* e = find_entry(table->entries, table->capacity, key);
    if (!e->occupied || !e->key) return false;
    *out = e->value;
    return true;
}

bool table_set(Table* table, ObjString* key, Value value) {
    const double MAX_LOAD = 0.75;
    if ((table->count + 1) > (int)(table->capacity * MAX_LOAD)) {
        int capacity = table->capacity < 8 ? 8 : table->capacity * 2;
        // Ensure power-of-two.
        adjust_capacity(table, capacity);
    }

    Entry* e = find_entry(table->entries, table->capacity, key);
    bool is_new = !e->occupied;
    if (is_new) {
        e->occupied = true;
        e->key = key;
        e->value = value;
        table->count++;
        return true;
    }

    // Update existing.
    e->key = key;
    e->value = value;
    return false;
}

ObjString* table_intern_string(Table* table, const char* chars, int length) {
    // For MVP, we don't implement full string interning table with tombstones.
    // We simply allocate a new ObjString and store it (dedup not required).
    ObjString* s = obj_string_copy(chars, length);
    if (!s) return NULL;
    table_set(table, s, NIL_VAL());
    return s;
}
