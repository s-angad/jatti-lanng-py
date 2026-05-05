#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object.h"

static void reset_stack(VM* vm) {
    vm->stack_top = vm->stack;
}

static void runtime_error(VM* vm, int line, const char* msg) {
    vm->runtime_error_line = line;
    vm->runtime_error_msg = msg;
}

static void push(VM* vm, Value v) {
    *vm->stack_top = v;
    vm->stack_top++;
}

static Value pop(VM* vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

static Value peek(VM* vm, int distance) {
    return vm->stack_top[-1 - distance];
}

static JattiStatus native_kism(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "kism expects 1 argument."; return JATTI_ERR; }
    const char* typeName = "unknown";
    if (IS_NIL(args[0])) typeName = "khaali";
    else if (IS_BOOL(args[0])) typeName = "bool";
    else if (IS_NUMBER(args[0])) typeName = "number";
    else if (IS_OBJ(args[0])) {
        Obj* obj = AS_OBJ(args[0]);
        if (obj->type == OBJ_STRING) typeName = "string";
        else if (obj->type == OBJ_FUNCTION) typeName = "kaam";
        else if (obj->type == OBJ_NATIVE) typeName = "native";
        else if (obj->type == OBJ_LIST) typeName = "list";
        else if (obj->type == OBJ_DICT) typeName = "dict";
    }
    ObjString* s = obj_string_copy(typeName, (int)strlen(typeName));
    if (!s) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(s);
    return JATTI_OK;
}

static JattiStatus native_likh(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 2) { *err = "likh expects 2 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING || !IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) {
        *err = "likh expects (string path, string text).";
        return JATTI_ERR;
    }

    ObjString* path = (ObjString*)AS_OBJ(args[0]);
    ObjString* text = (ObjString*)AS_OBJ(args[1]);
    FILE* f = fopen(path->chars, "wb");
    if (!f) { *out = BOOL_VAL(false); return JATTI_OK; }
    size_t written = fwrite(text->chars, 1, (size_t)text->length, f);
    fclose(f);
    *out = BOOL_VAL(written == (size_t)text->length);
    return JATTI_OK;
}

static JattiStatus native_padh(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "padh expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) {
        *err = "padh expects a string path.";
        return JATTI_ERR;
    }

    ObjString* path = (ObjString*)AS_OBJ(args[0]);
    FILE* f = fopen(path->chars, "rb");
    if (!f) { *out = NIL_VAL(); return JATTI_OK; }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz < 0) { fclose(f); *out = NIL_VAL(); return JATTI_OK; }

    char* buf = (char*)malloc((size_t)sz + 1);
    if (!buf) { fclose(f); *err = "Out of memory."; return JATTI_ERR; }
    size_t n = fread(buf, 1, (size_t)sz, f);
    fclose(f);
    buf[n] = '\0';

    ObjString* text = obj_string_copy(buf, (int)n);
    free(buf);
    if (!text) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(text);
    return JATTI_OK;
}

static bool list_all_numbers(ObjList* list) {
    for (int i = 0; i < list->count; i++) if (!IS_NUMBER(list->items[i])) return false;
    return true;
}

static bool list_all_strings(ObjList* list) {
    for (int i = 0; i < list->count; i++) {
        if (!IS_OBJ(list->items[i]) || AS_OBJ(list->items[i])->type != OBJ_STRING) return false;
    }
    return true;
}

static JattiStatus native_ganao(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "ganao expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_LIST) { *err = "ganao expects a list."; return JATTI_ERR; }
    ObjList* list = (ObjList*)AS_OBJ(args[0]);
    double sum = 0;
    for (int i = 0; i < list->count; i++) {
        if (!IS_NUMBER(list->items[i])) { *err = "ganao list must contain numbers."; return JATTI_ERR; }
        sum += AS_NUMBER(list->items[i]);
    }
    *out = NUMBER_VAL(sum);
    return JATTI_OK;
}

static JattiStatus native_max(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "sab_ton_vaddha expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_LIST) { *err = "sab_ton_vaddha expects a list."; return JATTI_ERR; }
    ObjList* list = (ObjList*)AS_OBJ(args[0]);
    if (list->count == 0) { *err = "sab_ton_vaddha requires non-empty list."; return JATTI_ERR; }
    if (!list_all_numbers(list)) { *err = "sab_ton_vaddha list must contain numbers."; return JATTI_ERR; }
    double best = AS_NUMBER(list->items[0]);
    for (int i = 1; i < list->count; i++) {
        double v = AS_NUMBER(list->items[i]);
        if (v > best) best = v;
    }
    *out = NUMBER_VAL(best);
    return JATTI_OK;
}

static JattiStatus native_min(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "sab_ton_chhota expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_LIST) { *err = "sab_ton_chhota expects a list."; return JATTI_ERR; }
    ObjList* list = (ObjList*)AS_OBJ(args[0]);
    if (list->count == 0) { *err = "sab_ton_chhota requires non-empty list."; return JATTI_ERR; }
    if (!list_all_numbers(list)) { *err = "sab_ton_chhota list must contain numbers."; return JATTI_ERR; }
    double best = AS_NUMBER(list->items[0]);
    for (int i = 1; i < list->count; i++) {
        double v = AS_NUMBER(list->items[i]);
        if (v < best) best = v;
    }
    *out = NUMBER_VAL(best);
    return JATTI_OK;
}

static JattiStatus native_sorted(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "sorted expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_LIST) { *err = "sorted expects a list."; return JATTI_ERR; }
    ObjList* src = (ObjList*)AS_OBJ(args[0]);
    ObjList* dst = obj_list_new();
    if (!dst) { *err = "Out of memory."; return JATTI_ERR; }
    for (int i = 0; i < src->count; i++) {
        if (!obj_list_push(dst, src->items[i])) { *err = "Out of memory."; return JATTI_ERR; }
    }

    if (list_all_numbers(dst)) {
        for (int i = 0; i < dst->count; i++) {
            for (int j = i + 1; j < dst->count; j++) {
                if (AS_NUMBER(dst->items[j]) < AS_NUMBER(dst->items[i])) {
                    Value t = dst->items[i];
                    dst->items[i] = dst->items[j];
                    dst->items[j] = t;
                }
            }
        }
    } else if (list_all_strings(dst)) {
        for (int i = 0; i < dst->count; i++) {
            for (int j = i + 1; j < dst->count; j++) {
                ObjString* a = (ObjString*)AS_OBJ(dst->items[i]);
                ObjString* b = (ObjString*)AS_OBJ(dst->items[j]);
                if (strcmp(b->chars, a->chars) < 0) {
                    Value t = dst->items[i];
                    dst->items[i] = dst->items[j];
                    dst->items[j] = t;
                }
            }
        }
    } else {
        *err = "sorted supports list of numbers or list of strings.";
        return JATTI_ERR;
    }

    *out = OBJ_VAL(dst);
    return JATTI_OK;
}

static JattiStatus native_reversed(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "reversed expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_LIST) { *err = "reversed expects a list."; return JATTI_ERR; }
    ObjList* src = (ObjList*)AS_OBJ(args[0]);
    ObjList* dst = obj_list_new();
    if (!dst) { *err = "Out of memory."; return JATTI_ERR; }
    for (int i = src->count - 1; i >= 0; i--) {
        if (!obj_list_push(dst, src->items[i])) { *err = "Out of memory."; return JATTI_ERR; }
    }
    *out = OBJ_VAL(dst);
    return JATTI_OK;
}

// String methods
static JattiStatus native_vada_likha(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "vada_likha expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "vada_likha expects a string."; return JATTI_ERR; }
    ObjString* src = (ObjString*)AS_OBJ(args[0]);
    char* buf = (char*)malloc((size_t)src->length + 1);
    if (!buf) { *err = "Out of memory."; return JATTI_ERR; }
    for (int i = 0; i < src->length; i++) {
        buf[i] = (char)((src->chars[i] >= 'a' && src->chars[i] <= 'z') ? src->chars[i] - 32 : src->chars[i]);
    }
    buf[src->length] = '\0';
    ObjString* result = obj_string_copy(buf, src->length);
    free(buf);
    if (!result) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_chhota_likha(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "chhota_likha expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "chhota_likha expects a string."; return JATTI_ERR; }
    ObjString* src = (ObjString*)AS_OBJ(args[0]);
    char* buf = (char*)malloc((size_t)src->length + 1);
    if (!buf) { *err = "Out of memory."; return JATTI_ERR; }
    for (int i = 0; i < src->length; i++) {
        buf[i] = (char)((src->chars[i] >= 'A' && src->chars[i] <= 'Z') ? src->chars[i] + 32 : src->chars[i]);
    }
    buf[src->length] = '\0';
    ObjString* result = obj_string_copy(buf, src->length);
    free(buf);
    if (!result) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_saf_karo(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 1) { *err = "saf_karo expects 1 argument."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "saf_karo expects a string."; return JATTI_ERR; }
    ObjString* src = (ObjString*)AS_OBJ(args[0]);
    int start = 0, end = src->length - 1;
    while (start <= end && (src->chars[start] == ' ' || src->chars[start] == '\t' || src->chars[start] == '\n')) start++;
    while (end >= start && (src->chars[end] == ' ' || src->chars[end] == '\t' || src->chars[end] == '\n')) end--;
    int len = (start <= end) ? (end - start + 1) : 0;
    ObjString* result = obj_string_copy(src->chars + start, len);
    if (!result) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_vand_karo(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 2) { *err = "vand_karo expects 2 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "vand_karo expects (string, delimiter)."; return JATTI_ERR; }
    if (!IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) { *err = "vand_karo expects (string, delimiter)."; return JATTI_ERR; }
    ObjString* str = (ObjString*)AS_OBJ(args[0]);
    ObjString* delim = (ObjString*)AS_OBJ(args[1]);
    ObjList* list = obj_list_new();
    if (!list) { *err = "Out of memory."; return JATTI_ERR; }
    if (delim->length == 0) {
        *err = "vand_karo delimiter cannot be empty.";
        return JATTI_ERR;
    }
    const char* p = str->chars;
    while (*p) {
        const char* match = strstr(p, delim->chars);
        int len = match ? (int)(match - p) : (int)strlen(p);
        ObjString* part = obj_string_copy(p, len);
        if (!part || !obj_list_push(list, OBJ_VAL(part))) { *err = "Out of memory."; return JATTI_ERR; }
        if (!match) break;
        p = match + delim->length;
    }
    *out = OBJ_VAL(list);
    return JATTI_OK;
}

static JattiStatus native_badal_de(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 3) { *err = "badal_de expects 3 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "badal_de expects (string, old, new)."; return JATTI_ERR; }
    if (!IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) { *err = "badal_de expects (string, old, new)."; return JATTI_ERR; }
    if (!IS_OBJ(args[2]) || AS_OBJ(args[2])->type != OBJ_STRING) { *err = "badal_de expects (string, old, new)."; return JATTI_ERR; }
    ObjString* str = (ObjString*)AS_OBJ(args[0]);
    ObjString* oldStr = (ObjString*)AS_OBJ(args[1]);
    ObjString* newStr = (ObjString*)AS_OBJ(args[2]);
    if (oldStr->length == 0) { *out = OBJ_VAL(str); return JATTI_OK; }
    char* buf = (char*)malloc(4096);
    if (!buf) { *err = "Out of memory."; return JATTI_ERR; }
    int pos = 0;
    const char* p = str->chars;
    while (*p && pos < 4095) {
        const char* match = strstr(p, oldStr->chars);
        if (!match) {
            int remaining = (int)strlen(p);
            if (pos + remaining > 4095) remaining = 4095 - pos;
            memcpy(buf + pos, p, (size_t)remaining);
            pos += remaining;
            break;
        }
        int copyLen = (int)(match - p);
        if (pos + copyLen > 4095) copyLen = 4095 - pos;
        memcpy(buf + pos, p, (size_t)copyLen);
        pos += copyLen;
        if (pos + newStr->length > 4095) break;
        memcpy(buf + pos, newStr->chars, (size_t)newStr->length);
        pos += newStr->length;
        p = match + oldStr->length;
    }
    buf[pos] = '\0';
    ObjString* result = obj_string_copy(buf, pos);
    free(buf);
    if (!result) { *err = "Out of memory."; return JATTI_ERR; }
    *out = OBJ_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_shuru_hunda(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 2) { *err = "shuru_hunda expects 2 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "shuru_hunda expects (string, prefix)."; return JATTI_ERR; }
    if (!IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) { *err = "shuru_hunda expects (string, prefix)."; return JATTI_ERR; }
    ObjString* str = (ObjString*)AS_OBJ(args[0]);
    ObjString* prefix = (ObjString*)AS_OBJ(args[1]);
    bool result = (str->length >= prefix->length) && (strncmp(str->chars, prefix->chars, (size_t)prefix->length) == 0);
    *out = BOOL_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_khatam_hunda(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 2) { *err = "khatam_hunda expects 2 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "khatam_hunda expects (string, suffix)."; return JATTI_ERR; }
    if (!IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) { *err = "khatam_hunda expects (string, suffix)."; return JATTI_ERR; }
    ObjString* str = (ObjString*)AS_OBJ(args[0]);
    ObjString* suffix = (ObjString*)AS_OBJ(args[1]);
    bool result = (str->length >= suffix->length) && (strcmp(str->chars + (str->length - suffix->length), suffix->chars) == 0);
    *out = BOOL_VAL(result);
    return JATTI_OK;
}

static JattiStatus native_dhundh_ja(VM* vm, int argCount, Value* args, Value* out, const char** err) {
    JATTI_UNUSED(vm);
    if (argCount != 2) { *err = "dhundh_ja expects 2 arguments."; return JATTI_ERR; }
    if (!IS_OBJ(args[0]) || AS_OBJ(args[0])->type != OBJ_STRING) { *err = "dhundh_ja expects (string, substr)."; return JATTI_ERR; }
    if (!IS_OBJ(args[1]) || AS_OBJ(args[1])->type != OBJ_STRING) { *err = "dhundh_ja expects (string, substr)."; return JATTI_ERR; }
    ObjString* str = (ObjString*)AS_OBJ(args[0]);
    ObjString* substr = (ObjString*)AS_OBJ(args[1]);
    const char* match = strstr(str->chars, substr->chars);
    int idx = match ? (int)(match - str->chars) : -1;
    *out = NUMBER_VAL((double)idx);
    return JATTI_OK;
}

static bool define_native(VM* vm, const char* name, int arity, NativeFn function) {
    ObjString* nameObj = table_intern_string(&vm->strings, name, (int)strlen(name));
    if (!nameObj) return false;
    ObjNative* native = obj_native_new(nameObj, arity, function);
    if (!native) return false;
    table_set(&vm->globals, nameObj, OBJ_VAL(native));
    return true;
}

void vm_init(VM* vm) {
    vm->chunk = NULL;
    vm->ip = NULL;
    reset_stack(vm);
    vm->frame_count = 0;
    vm->try_count = 0;
    table_init(&vm->globals);
    table_init(&vm->strings);
    vm->objects = NULL;
    obj_set_vm_object_list(&vm->objects);
    arena_init(&vm->scratch);
    vm->runtime_error_line = 0;
    vm->runtime_error_msg = NULL;

    define_native(vm, "kism", 1, native_kism);
    define_native(vm, "likh", 2, native_likh);
    define_native(vm, "padh", 1, native_padh);
    define_native(vm, "ganao", 1, native_ganao);
    define_native(vm, "sab_ton_vaddha", 1, native_max);
    define_native(vm, "sab_ton_chhota", 1, native_min);
    define_native(vm, "sorted", 1, native_sorted);
    define_native(vm, "chal_sort_hoja", 1, native_sorted);  // Punjabi alias for sorted
    define_native(vm, "reversed", 1, native_reversed);
    define_native(vm, "chal_reverse_hoja", 1, native_reversed);  // Punjabi alias for reversed
    
    // String methods
    define_native(vm, "vada_likha", 1, native_vada_likha);
    define_native(vm, "chhota_likha", 1, native_chhota_likha);
    define_native(vm, "saf_karo", 1, native_saf_karo);
    define_native(vm, "vand_karo", 2, native_vand_karo);
    define_native(vm, "badal_de", 3, native_badal_de);
    define_native(vm, "shuru_hunda", 2, native_shuru_hunda);
    define_native(vm, "khatam_hunda", 2, native_khatam_hunda);
    define_native(vm, "dhundh_ja", 2, native_dhundh_ja);
}

void vm_free(VM* vm) {
    for (int i = 0; i < vm->frame_count; i++) {
        table_free(&vm->frames[i].locals);
    }

    table_free(&vm->globals);
    table_free(&vm->strings);

    // Free objects.
    Obj* obj = vm->objects;
    while (obj) {
        Obj* next = obj->next;
        if (obj->type == OBJ_STRING) {
            ObjString* s = (ObjString*)obj;
            free(s->chars);
        } else if (obj->type == OBJ_FUNCTION) {
            ObjFunction* fn = (ObjFunction*)obj;
            free(fn->params);
            chunk_free(&fn->chunk);
        } else if (obj->type == OBJ_NATIVE) {
            // no additional heap allocations
        } else if (obj->type == OBJ_LIST) {
            ObjList* list = (ObjList*)obj;
            free(list->items);
        } else if (obj->type == OBJ_DICT) {
            ObjDict* dict = (ObjDict*)obj;
            free(dict->keys);
            free(dict->values);
        }
        free(obj);
        obj = next;
    }

    arena_free(&vm->scratch);
}

static CallFrame* current_frame(VM* vm) {
    if (vm->frame_count <= 0) return NULL;
    return &vm->frames[vm->frame_count - 1];
}

static bool get_var(VM* vm, ObjString* name, Value* out) {
    CallFrame* frame = current_frame(vm);
    if (frame && !frame->is_script) {
        if (table_get(&frame->locals, name, out)) return true;
    }
    return table_get(&vm->globals, name, out);
}

static void set_var(VM* vm, ObjString* name, Value value) {
    CallFrame* frame = current_frame(vm);
    if (frame && !frame->is_script) {
        table_set(&frame->locals, name, value);
        return;
    }
    table_set(&vm->globals, name, value);
}

static void prune_try_handlers(VM* vm) {
    while (vm->try_count > 0 && vm->try_handlers[vm->try_count - 1].frame_index >= vm->frame_count) {
        vm->try_count--;
    }
}

static bool handle_thrown_error(VM* vm, int line) {
    if (vm->try_count <= 0) return false;

    TryHandler handler = vm->try_handlers[--vm->try_count];

    while (vm->frame_count - 1 > handler.frame_index) {
        table_free(&vm->frames[vm->frame_count - 1].locals);
        vm->frame_count--;
    }

    prune_try_handlers(vm);

    if (handler.frame_index < 0 || handler.frame_index >= vm->frame_count) {
        return false;
    }

    CallFrame* frame = &vm->frames[handler.frame_index];
    if (!frame->chunk || !frame->chunk->code) {
        runtime_error(vm, line, "Invalid error handler.");
        return false;
    }

    u8* code_start = frame->chunk->code;
    u8* code_end = frame->chunk->code + frame->chunk->count;
    if (handler.handler_ip < code_start || handler.handler_ip >= code_end) {
        runtime_error(vm, line, "Invalid error handler.");
        return false;
    }
    frame->ip = handler.handler_ip;

    const char* msg = vm->runtime_error_msg ? vm->runtime_error_msg : "Runtime error.";
    ObjString* msgObj = obj_string_copy(msg, (int)strlen(msg));
    if (!msgObj) {
        runtime_error(vm, line, "Out of memory.");
        return false;
    }

    ObjString* internalErrName = table_intern_string(&vm->strings, "__jatti_err", 11);
    if (!internalErrName) {
        runtime_error(vm, line, "Out of memory.");
        return false;
    }
    table_set(&vm->globals, internalErrName, OBJ_VAL(msgObj));
    reset_stack(vm);
    vm->runtime_error_line = 0;
    vm->runtime_error_msg = NULL;
    return true;
}

static bool call_function(VM* vm, ObjFunction* fn, int argCount, int line) {
    if (argCount != fn->arity) {
        runtime_error(vm, line, "Function argument count mismatch.");
        return false;
    }
    if (vm->frame_count >= FRAMES_MAX) {
        runtime_error(vm, line, "Call stack overflow.");
        return false;
    }

    CallFrame* frame = &vm->frames[vm->frame_count++];
    frame->chunk = &fn->chunk;
    frame->ip = fn->chunk.code;
    frame->is_script = false;
    table_init(&frame->locals);

    Value* argStart = vm->stack_top - argCount;
    for (int i = 0; i < argCount; i++) {
        table_set(&frame->locals, fn->params[i], argStart[i]);
    }

    vm->stack_top -= (argCount + 1); // remove callee + args
    vm->chunk = frame->chunk;
    vm->ip = frame->ip;
    return true;
}

static bool binary_num(VM* vm, OpCode op, int line) {
    Value b = pop(vm);
    Value a = pop(vm);
    // OP_ADD also supports string concatenation for MVP.
    if (op == OP_ADD && IS_OBJ(a) && IS_OBJ(b) && AS_OBJ(a)->type == OBJ_STRING && AS_OBJ(b)->type == OBJ_STRING) {
        ObjString* sa = (ObjString*)AS_OBJ(a);
        ObjString* sb = (ObjString*)AS_OBJ(b);
        int newLen = sa->length + sb->length;
        char* buf = (char*)malloc((size_t)newLen + 1);
        if (!buf) {
            runtime_error(vm, line, "Out of memory.");
            return false;
        }
        memcpy(buf, sa->chars, (size_t)sa->length);
        memcpy(buf + sa->length, sb->chars, (size_t)sb->length);
        buf[newLen] = '\0';
        ObjString* out = obj_string_copy(buf, newLen);
        free(buf);
        if (!out) {
            runtime_error(vm, line, "Out of memory.");
            return false;
        }
        push(vm, OBJ_VAL(out));
        return true;
    }

    if (op == OP_ADD && IS_OBJ(a) && IS_OBJ(b) && AS_OBJ(a)->type == OBJ_LIST && AS_OBJ(b)->type == OBJ_LIST) {
        ObjList* la = (ObjList*)AS_OBJ(a);
        ObjList* lb = (ObjList*)AS_OBJ(b);
        ObjList* out = obj_list_new();
        if (!out) {
            runtime_error(vm, line, "Out of memory.");
            return false;
        }
        for (int i = 0; i < la->count; i++) {
            if (!obj_list_push(out, la->items[i])) { runtime_error(vm, line, "Out of memory."); return false; }
        }
        for (int i = 0; i < lb->count; i++) {
            if (!obj_list_push(out, lb->items[i])) { runtime_error(vm, line, "Out of memory."); return false; }
        }
        push(vm, OBJ_VAL(out));
        return true;
    }

    if (!IS_NUMBER(a) || !IS_NUMBER(b)) {
        runtime_error(vm, line, "Number expected.");
        return false;
    }
    double x = AS_NUMBER(a);
    double y = AS_NUMBER(b);
    switch (op) {
        case OP_ADD: push(vm, NUMBER_VAL(x + y)); break;
        case OP_SUB: push(vm, NUMBER_VAL(x - y)); break;
        case OP_MUL: push(vm, NUMBER_VAL(x * y)); break;
        case OP_DIV:
            if (y == 0.0) {
                runtime_error(vm, line, "Cannot divide by zero.");
                return false;
            }
            push(vm, NUMBER_VAL(x / y));
            break;
        case OP_MOD: {
            long long ix = (long long)x;
            long long iy = (long long)y;
            if (iy == 0) { runtime_error(vm, line, "Modulo by zero."); return false; }
            push(vm, NUMBER_VAL((double)(ix % iy)));
            break;
        }
        case OP_POW: {
            push(vm, NUMBER_VAL(pow(x, y)));
            break;
        }
        default: return false;
    }
    return true;
}

static bool compare_num(VM* vm, OpCode op, int line) {
    Value b = pop(vm);
    Value a = pop(vm);
    if (!IS_NUMBER(a) || !IS_NUMBER(b)) {
        runtime_error(vm, line, "Number expected.");
        return false;
    }
    double x = AS_NUMBER(a);
    double y = AS_NUMBER(b);
    switch (op) {
        case OP_LT: push(vm, BOOL_VAL(x < y)); break;
        case OP_LTE: push(vm, BOOL_VAL(x <= y)); break;
        case OP_GT: push(vm, BOOL_VAL(x > y)); break;
        case OP_GTE: push(vm, BOOL_VAL(x >= y)); break;
        default: return false;
    }
    return true;
}

JattiStatus vm_run(VM* vm, Chunk* chunk) {
    vm->frame_count = 1;
    vm->frames[0].chunk = chunk;
    vm->frames[0].ip = chunk->code;
    vm->frames[0].is_script = true;
    table_init(&vm->frames[0].locals);

    vm->chunk = chunk;
    vm->ip = chunk->code;
    vm->runtime_error_line = 0;
    vm->runtime_error_msg = NULL;
    vm->try_count = 0;

    for (;;) {
        CallFrame* frame = current_frame(vm);
        if (!frame) return JATTI_OK;

        vm->chunk = frame->chunk;
        vm->ip = frame->ip;

        u8 instruction = *frame->ip++;
        frame->ip = frame->ip;
        int line = frame->chunk->lines[(int)(frame->ip - frame->chunk->code - 1)];

        switch ((OpCode)instruction) {
            case OP_CONST: {
                u16 idx = (u16)(*frame->ip++ << 8);
                idx |= *frame->ip++;
                push(vm, frame->chunk->constants[idx]);
                break;
            }
            case OP_NIL: push(vm, NIL_VAL()); break;
            case OP_TRUE: push(vm, BOOL_VAL(true)); break;
            case OP_FALSE: push(vm, BOOL_VAL(false)); break;
            case OP_POP: pop(vm); break;

            case OP_GET_GLOBAL: {
                u16 idx = (u16)(*frame->ip++ << 8);
                idx |= *frame->ip++;
                Value nameVal = frame->chunk->constants[idx];
                ObjString* name = (ObjString*)AS_OBJ(nameVal);
                Value out;
                if (!get_var(vm, name, &out)) {
                    runtime_error(vm, line, "Undefined variable.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                push(vm, out);
                break;
            }
            case OP_SET_GLOBAL: {
                u16 idx = (u16)(*frame->ip++ << 8);
                idx |= *frame->ip++;
                Value nameVal = frame->chunk->constants[idx];
                ObjString* name = (ObjString*)AS_OBJ(nameVal);
                Value val = peek(vm, 0);
                set_var(vm, name, val);
                break;
            }

            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
                if (!binary_num(vm, (OpCode)instruction, line)) {
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                break;

            case OP_NEG: {
                Value v = pop(vm);
                if (!IS_NUMBER(v)) {
                    runtime_error(vm, line, "Number expected.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                push(vm, NUMBER_VAL(-AS_NUMBER(v)));
                break;
            }
            case OP_NOT: {
                Value v = pop(vm);
                push(vm, BOOL_VAL(value_is_falsey(v)));
                break;
            }

            case OP_EQ: {
                Value b = pop(vm);
                Value a = pop(vm);
                push(vm, BOOL_VAL(value_equals(a, b)));
                break;
            }
            case OP_NEQ: {
                Value b = pop(vm);
                Value a = pop(vm);
                push(vm, BOOL_VAL(!value_equals(a, b)));
                break;
            }

            case OP_LT:
            case OP_LTE:
            case OP_GT:
            case OP_GTE:
                if (!compare_num(vm, (OpCode)instruction, line)) {
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                break;

            case OP_LIST: {
                int itemCount = (int)(*frame->ip++);
                ObjList* list = obj_list_new();
                if (!list) {
                    runtime_error(vm, line, "Out of memory.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                Value* temp = NULL;
                if (itemCount > 0) {
                    temp = (Value*)malloc((size_t)itemCount * sizeof(Value));
                    if (!temp) {
                        runtime_error(vm, line, "Out of memory.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                }
                for (int i = itemCount - 1; i >= 0; i--) temp[i] = pop(vm);
                for (int i = 0; i < itemCount; i++) {
                    if (!obj_list_push(list, temp[i])) {
                        free(temp);
                        runtime_error(vm, line, "Out of memory.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                }
                free(temp);
                push(vm, OBJ_VAL(list));
                break;
            }

            case OP_DICT: {
                int pairCount = (int)(*frame->ip++);
                ObjDict* dict = obj_dict_new();
                if (!dict) {
                    runtime_error(vm, line, "Out of memory.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                for (int i = pairCount - 1; i >= 0; i--) {
                    Value value = pop(vm);
                    Value keyVal = pop(vm);
                    if (!IS_OBJ(keyVal) || AS_OBJ(keyVal)->type != OBJ_STRING) {
                        runtime_error(vm, line, "Dictionary key must be string.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    if (!obj_dict_set(dict, (ObjString*)AS_OBJ(keyVal), value)) {
                        runtime_error(vm, line, "Out of memory.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                }
                push(vm, OBJ_VAL(dict));
                break;
            }

            case OP_GET_INDEX: {
                Value index = pop(vm);
                Value container = pop(vm);

                if (!IS_OBJ(container)) {
                    runtime_error(vm, line, "Indexing requires list, dict, or string.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }

                Obj* cobj = AS_OBJ(container);
                if (cobj->type == OBJ_LIST) {
                    if (!IS_NUMBER(index)) {
                        runtime_error(vm, line, "List index must be number.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    int idx = (int)AS_NUMBER(index);
                    ObjList* list = (ObjList*)cobj;
                    if (idx < 0 || idx >= list->count) {
                        runtime_error(vm, line, "List index out of range.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    push(vm, list->items[idx]);
                    break;
                }

                if (cobj->type == OBJ_DICT) {
                    ObjDict* dict = (ObjDict*)cobj;
                    if (IS_NUMBER(index)) {
                        int idx = (int)AS_NUMBER(index);
                        if (idx < 0 || idx >= dict->count) {
                            runtime_error(vm, line, "Dict index out of range.");
                            if (handle_thrown_error(vm, line)) break;
                            return JATTI_ERR;
                        }
                        push(vm, OBJ_VAL(dict->keys[idx]));
                    } else if (IS_OBJ(index) && AS_OBJ(index)->type == OBJ_STRING) {
                        Value out;
                        if (!obj_dict_get(dict, (ObjString*)AS_OBJ(index), &out)) {
                            runtime_error(vm, line, "Key not found.");
                            if (handle_thrown_error(vm, line)) break;
                            return JATTI_ERR;
                        }
                        push(vm, out);
                    } else {
                        runtime_error(vm, line, "Dict index must be string key or numeric position.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    break;
                }

                if (cobj->type == OBJ_STRING) {
                    if (!IS_NUMBER(index)) {
                        runtime_error(vm, line, "String index must be number.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    ObjString* str = (ObjString*)cobj;
                    int idx = (int)AS_NUMBER(index);
                    if (idx < 0 || idx >= str->length) {
                        runtime_error(vm, line, "String index out of range.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    char one[2];
                    one[0] = str->chars[idx];
                    one[1] = '\0';
                    ObjString* out = obj_string_copy(one, 1);
                    if (!out) {
                        runtime_error(vm, line, "Out of memory.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    push(vm, OBJ_VAL(out));
                    break;
                }

                runtime_error(vm, line, "Unsupported index target.");
                if (handle_thrown_error(vm, line)) break;
                return JATTI_ERR;
            }

            case OP_LEN: {
                Value v = pop(vm);
                if (!IS_OBJ(v)) {
                    runtime_error(vm, line, "kinna_lamba expects string/list/dict.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                Obj* obj = AS_OBJ(v);
                if (obj->type == OBJ_STRING) {
                    push(vm, NUMBER_VAL((double)((ObjString*)obj)->length));
                } else if (obj->type == OBJ_LIST) {
                    push(vm, NUMBER_VAL((double)((ObjList*)obj)->count));
                } else if (obj->type == OBJ_DICT) {
                    push(vm, NUMBER_VAL((double)((ObjDict*)obj)->count));
                } else {
                    runtime_error(vm, line, "kinna_lamba expects string/list/dict.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                break;
            }

            case OP_JMP: {
                u16 off = (u16)(*frame->ip++ << 8);
                off |= *frame->ip++;
                frame->ip += off;
                break;
            }
            case OP_JMP_IF_FALSE: {
                u16 off = (u16)(*frame->ip++ << 8);
                off |= *frame->ip++;
                Value cond = peek(vm, 0);
                if (value_is_falsey(cond)) {
                    frame->ip += off;
                }
                break;
            }
            case OP_LOOP: {
                u16 off = (u16)(*frame->ip++ << 8);
                off |= *frame->ip++;
                frame->ip -= off;
                break;
            }

            case OP_CALL: {
                int argCount = (int)(*frame->ip++);
                Value callee = peek(vm, argCount);
                if (!IS_OBJ(callee)) {
                    runtime_error(vm, line, "Can only call kaam values.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }

                Obj* calleeObj = AS_OBJ(callee);
                if (calleeObj->type == OBJ_FUNCTION) {
                    if (!call_function(vm, (ObjFunction*)calleeObj, argCount, line)) {
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                } else if (calleeObj->type == OBJ_NATIVE) {
                    ObjNative* native = (ObjNative*)calleeObj;
                    if (native->arity >= 0 && argCount != native->arity) {
                        runtime_error(vm, line, "Function argument count mismatch.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    Value* args = vm->stack_top - argCount;
                    Value out = NIL_VAL();
                    const char* err = NULL;
                    if (native->function(vm, argCount, args, &out, &err) != JATTI_OK) {
                        runtime_error(vm, line, err ? err : "Native function failed.");
                        if (handle_thrown_error(vm, line)) break;
                        return JATTI_ERR;
                    }
                    vm->stack_top -= (argCount + 1);
                    push(vm, out);
                } else {
                    runtime_error(vm, line, "Can only call kaam values.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }
                break;
            }

            case OP_TRY: {
                u16 off = (u16)(*frame->ip++ << 8);
                off |= *frame->ip++;

                if (vm->try_count >= TRY_MAX) {
                    runtime_error(vm, line, "Too many nested try blocks.");
                    if (handle_thrown_error(vm, line)) break;
                    return JATTI_ERR;
                }

                vm->try_handlers[vm->try_count].frame_index = vm->frame_count - 1;
                vm->try_handlers[vm->try_count].handler_ip = frame->ip + off;
                vm->try_count++;
                break;
            }

            case OP_END_TRY: {
                if (vm->try_count > 0 && vm->try_handlers[vm->try_count - 1].frame_index == vm->frame_count - 1) {
                    vm->try_count--;
                }
                break;
            }

            case OP_RETURN: {
                Value ret = pop(vm);
                table_free(&frame->locals);
                vm->frame_count--;
                prune_try_handlers(vm);

                if (vm->frame_count == 0) {
                    push(vm, ret);
                    return JATTI_OK;
                }

                push(vm, ret);
                break;
            }

            case OP_PRINT: {
                Value v = pop(vm);
                value_print(v);
                printf("\n");
                break;
            }

            case OP_HALT:
                table_free(&vm->frames[0].locals);
                vm->frame_count = 0;
                vm->try_count = 0;
                return JATTI_OK;
        }
    }
}
