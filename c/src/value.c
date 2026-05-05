#include "value.h"

#include <stdio.h>

#include "object.h"

bool value_is_falsey(Value v) {
    if (IS_NIL(v)) return true;
    if (IS_BOOL(v)) return !AS_BOOL(v);
    return false;
}

bool value_equals(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_NIL: return true;
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ: return obj_equals(AS_OBJ(a), AS_OBJ(b));
        default: return false;
    }
}

void value_print(Value v) {
    switch (v.type) {
        case VAL_NIL:
            printf("khaali");
            break;
        case VAL_BOOL:
            printf(AS_BOOL(v) ? "sach" : "jhoot");
            break;
        case VAL_NUMBER: {
            double n = AS_NUMBER(v);
            // Simple numeric printing: drop .0 when possible.
            if (n == (double)(long long)n) {
                printf("%lld", (long long)n);
            } else {
                printf("%.15g", n);
            }
            break;
        }
        case VAL_OBJ:
            obj_print(AS_OBJ(v));
            break;
    }
}
