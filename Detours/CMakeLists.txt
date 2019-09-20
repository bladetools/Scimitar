cmake_minimum_required(VERSION 3.4.1)

add_library(DetoursDisassembler
    src/disasm.cpp
    src/detours.cpp
    src/detours_x.cpp
)

target_include_directories(DetoursDisassembler PUBLIC src)
target_compile_definitions(DetoursDisassembler PRIVATE -DDETOURS_X=1)
