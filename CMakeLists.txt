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
        "src/global_header.h"
        "src/resources/lethal_company_icecream_music.binh"
        "src/resources/vine_boom_sfx.binh"
        src/audio/audio.c
        src/audio/audio.h)

target_link_libraries(custom_shaped_window PRIVATE winmm.lib)

target_include_directories(custom_shaped_window PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_options(custom_shaped_window PRIVATE
        -O0
        -imacros ${PROJECT_SOURCE_DIR}/src/global_header.h)
