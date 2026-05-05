cmake_minimum_required(VERSION 3.20)
project(jatti_c LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(jatti
    src/main.c
    src/common.h
    src/arena.c
    src/arena.h
    src/value.c
    src/value.h
    src/chunk.c
    src/chunk.h
    src/object.c
    src/object.h
    src/table.c
    src/table.h
    src/vm.c
    src/vm.h
    src/jatti_source.c
    src/jatti_source.h
    src/expr_scan.c
    src/expr_scan.h
    src/expr_parse.c
    src/expr_parse.h
    src/compiler.c
    src/compiler.h
)

if (MSVC)
  target_compile_definitions(jatti PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_options(jatti PRIVATE /W4)
else()
  target_compile_options(jatti PRIVATE -Wall -Wextra -Wpedantic)
endif()
