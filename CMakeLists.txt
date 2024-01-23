cmake_minimum_required(VERSION 3.27)
project(custom_shaped_window C)

set(CMAKE_C_STANDARD 11)

add_executable(custom_shaped_window
        "src/main.c"
        "src/window.c"
        "src/window.h"
        "src/globals.c"
        "src/globals.h"
        "src/amogus/amogus.c"
        "src/amogus/amogus.h"
        "src/data structures/vec2/vec2.h"
        "src/data structures/vec2/vec2.c"
        src/global_header.h)

target_include_directories(custom_shaped_window PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_options(custom_shaped_window PRIVATE
        -O0
        -imacros ${PROJECT_SOURCE_DIR}/src/global_header.h)
