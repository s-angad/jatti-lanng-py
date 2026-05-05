#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunk.h"

static Obj** g_vm_objects = NULL;

static void link_object(Obj* obj) {
    if (g_vm_objects) {
        obj->next = *g_vm_objects;
        *g_vm_objects = obj;
    } else {
        obj->next = NULL;
    }
}

void obj_set_vm_object_list(Obj** head) {
    g_vm_objects = head;
}

u32 hash_string(const char* key, int length) {
    // FNV-1a 32-bit
    u32 hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (u8)key[i];
        hash *= 16777619u;
    }
    return hash;
}

static ObjString* allocate_string(char* chars, int length, u32 hash) {
    ObjString* string = (ObjString*)malloc(sizeof(ObjString));
    if (!string) return NULL;
    string->obj.type = OBJ_STRING;
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    link_object((Obj*)string);

    return string;
}

ObjString* obj_string_copy(const char* chars, int length) {
    char* heap = (char*)malloc((size_t)length + 1);
    if (!heap) return NULL;
    memcpy(heap, chars, (size_t)length);
    heap[length] = '\0';
    u32 h = hash_string(heap, length);
    return allocate_string(heap, length, h);
}

ObjFunction* obj_function_new(ObjString* name, int arity, ObjString** params) {
    ObjFunction* fn = (ObjFunction*)malloc(sizeof(ObjFunction));
    if (!fn) return NULL;
    fn->obj.type = OBJ_FUNCTION;
    fn->arity = arity;
    fn->name = name;
    fn->params = params;
    chunk_init(&fn->chunk);

    link_object((Obj*)fn);

    return fn;
}

ObjNative* obj_native_new(ObjString* name, int arity, NativeFn function) {
    ObjNative* native = (ObjNative*)malloc(sizeof(ObjNative));
    if (!native) return NULL;
    native->obj.type = OBJ_NATIVE;
    native->arity = arity;
    native->name = name;
    native->function = function;
    link_object((Obj*)native);
    return native;
}

ObjList* obj_list_new(void) {
    ObjList* list = (ObjList*)malloc(sizeof(ObjList));
    if (!list) return NULL;
    list->obj.type = OBJ_LIST;
    list->count = 0;
    list->capacity = 0;
    list->items = NULL;
    link_object((Obj*)list);
    return list;
}

bool obj_list_push(ObjList* list, Value v) {
    if (list->count + 1 > list->capacity) {
        int newCap = list->capacity < 8 ? 8 : list->capacity * 2;
        Value* newItems = (Value*)realloc(list->items, (size_t)newCap * sizeof(Value));
        if (!newItems) return false;
        list->items = newItems;
        list->capacity = newCap;
    }
    list->items[list->count++] = v;
    return true;
}

ObjDict* obj_dict_new(void) {
    ObjDict* dict = (ObjDict*)malloc(sizeof(ObjDict));
    if (!dict) return NULL;
    dict->obj.type = OBJ_DICT;
    dict->count = 0;
    dict->capacity = 0;
    dict->keys = NULL;
    dict->values = NULL;
    link_object((Obj*)dict);
    return dict;
}

static bool dict_set(ObjDict* dict, ObjString* key, Value value) {
    for (int i = 0; i < dict->count; i++) {
        ObjString* k = dict->keys[i];
        if (k->length == key->length && memcmp(k->chars, key->chars, (size_t)k->length) == 0) {
            dict->values[i] = value;
            return true;
        }
    }

    if (dict->count + 1 > dict->capacity) {
        int newCap = dict->capacity < 8 ? 8 : dict->capacity * 2;
        ObjString** newKeys = (ObjString**)realloc(dict->keys, (size_t)newCap * sizeof(ObjString*));
        if (!newKeys) return false;
        Value* newVals = (Value*)realloc(dict->values, (size_t)newCap * sizeof(Value));
        if (!newVals) return false;
        dict->keys = newKeys;
        dict->values = newVals;
        dict->capacity = newCap;
    }

    dict->keys[dict->count] = key;
    dict->values[dict->count] = value;
    dict->count++;
    return true;
}

bool obj_dict_set(ObjDict* dict, ObjString* key, Value value) {
    return dict_set(dict, key, value);
}

bool obj_dict_get(ObjDict* dict, ObjString* key, Value* out) {
    for (int i = 0; i < dict->count; i++) {
        ObjString* k = dict->keys[i];
        if (k->length == key->length && memcmp(k->chars, key->chars, (size_t)k->length) == 0) {
            *out = dict->values[i];
            return true;
        }
    }
    return false;
}

bool obj_equals(Obj* a, Obj* b) {
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->type != b->type) return false;
    switch (a->type) {
        case OBJ_STRING: {
            ObjString* sa = (ObjString*)a;
            ObjString* sb = (ObjString*)b;
            if (sa->length != sb->length) return false;
            return memcmp(sa->chars, sb->chars, (size_t)sa->length) == 0;
        }
        case OBJ_FUNCTION:
            return a == b;
        case OBJ_NATIVE:
            return a == b;
        case OBJ_LIST:
        case OBJ_DICT:
            return a == b;
        default:
            return false;
    }
}

void obj_print(Obj* obj) {
    if (!obj) return;
    switch (obj->type) {
        case OBJ_STRING:
            printf("%s", ((ObjString*)obj)->chars);
            break;
        case OBJ_FUNCTION: {
            ObjFunction* fn = (ObjFunction*)obj;
            if (fn->name && fn->name->chars) {
                printf("<kaam %s>", fn->name->chars);
            } else {
                printf("<kaam>");
            }
            break;
        }
        case OBJ_NATIVE: {
            ObjNative* native = (ObjNative*)obj;
            if (native->name && native->name->chars) {
                printf("<native %s>", native->name->chars);
            } else {
                printf("<native>");
            }
            break;
        }
        case OBJ_LIST: {
            ObjList* list = (ObjList*)obj;
            printf("[");
            for (int i = 0; i < list->count; i++) {
                if (i > 0) printf(", ");
                value_print(list->items[i]);
            }
            printf("]");
            break;
        }
        case OBJ_DICT: {
            ObjDict* dict = (ObjDict*)obj;
            printf("{");
            for (int i = 0; i < dict->count; i++) {
                if (i > 0) printf(", ");
                printf("\"");
                printf("%s", dict->keys[i]->chars);
                printf("\": ");
                value_print(dict->values[i]);
            }
            printf("}");
            break;
        }
    }
}
