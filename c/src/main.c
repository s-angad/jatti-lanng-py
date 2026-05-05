#include "common.h"

#include <stdio.h>
#include <string.h>

#include "vm.h"
#include "compiler.h"

static const char* ROASTS[] = {
    "Galti ho gayi !! koi gall nahi.",
    "Dhyaan de, Jatti style rakhi !!",
    "Phir ohi mistake !!",
    "Tu compiler nu test kar reha !!",
    "Compiler thak gaya. Tu vi !!",
    "Tu coding chadd de !!",
    "Tere to nhi hona oye !!",
    "Eh ki likh ta tu?",
    "Dimag use kar le thoda.",
    "Jatti Lang mazak nahi hai.",
    "Syntax nu respect de.",
};

static const char* pick_roast(int line, const char* msg) {
    unsigned int hash = (unsigned int)(line > 0 ? line : 1);
    if (msg) {
        for (const char* p = msg; *p; p++) {
            hash = (hash * 33u) ^ (unsigned char)(*p);
        }
    }
    size_t count = sizeof(ROASTS) / sizeof(ROASTS[0]);
    return ROASTS[hash % count];
}

// Exception type identification
static const char* identify_exception_type(const char* msg) {
    if (!msg) return "JattiException";
    if (strstr(msg, "divide")) return "JattiZeroDivisionError";
    if (strstr(msg, "Undefined")) return "JattiNameError";
    if (strstr(msg, "index")) return "JattiIndexError";
    if (strstr(msg, "operand") || strstr(msg, "operator") || strstr(msg, "expected") || strstr(msg, "Number") || strstr(msg, "String")) return "JattiTypeError";
    if (strstr(msg, "Key") || strstr(msg, "key")) return "JattiKeyError";
    if (strstr(msg, "argument")) return "JattiArgumentError";
    if (strstr(msg, "syntax") || strstr(msg, "must start") || strstr(msg, "Unsupported")) return "JattiSyntaxError";
    return "JattiRuntimeError";
}

static void usage(void) {
    printf("jatti (C compiler/VM MVP)\n");
    printf("Usage:\n");
    printf("  jatti run <file.jatti>\n");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        usage();
        return 1;
    }

    const char* cmd = argv[1];
    const char* path = argv[2];

    if (strcmp(cmd, "run") != 0) {
        usage();
        return 1;
    }

    VM vm;
    vm_init(&vm);

    Chunk chunk;
    CompileError err;
    if (jatti_compile_file(&vm, path, &chunk, &err) != JATTI_OK) {
        const char* roast = pick_roast(err.line, err.message);
        const char* exType = identify_exception_type(err.message);
        fprintf(stderr, "🔥 Roast: %s\n", roast);
        if (err.line > 0) {
            fprintf(stderr, "❌ JATTI ERROR [%s]\n", exType);
            fprintf(stderr, "🔴 Error: %s\n", err.message);
            fprintf(stderr, "📍 Line %d\n", err.line);
        } else {
            fprintf(stderr, "❌ JATTI ERROR [%s]\n", exType);
            fprintf(stderr, "🔴 Error: %s\n", err.message);
        }
        vm_free(&vm);
        return 1;
    }

    JattiStatus st = vm_run(&vm, &chunk);
    if (st != JATTI_OK) {
        const char* runtimeMsg = vm.runtime_error_msg ? vm.runtime_error_msg : "Unknown";
        const char* roast = pick_roast(vm.runtime_error_line, runtimeMsg);
        const char* exType = identify_exception_type(runtimeMsg);
        fprintf(stderr, "🔥 Roast: %s\n", roast);
        fprintf(stderr, "❌ JATTI ERROR [%s]\n", exType);
        fprintf(stderr, "🔴 Error: %s\n", runtimeMsg);
        if (vm.runtime_error_line > 0) {
            fprintf(stderr, "📍 Line %d\n", vm.runtime_error_line);
        }
        vm_free(&vm);
        chunk_free(&chunk);
        return 1;
    }

    chunk_free(&chunk);
    vm_free(&vm);
    return 0;
}
