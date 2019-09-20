cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(Detours)

add_library(Scimitar ProcMaps.cpp DetoursNix.cpp)
target_link_libraries(Scimitar PRIVATE DetoursDisassembler)
target_link_libraries(Scimitar PUBLIC dl)
target_include_directories(Scimitar PUBLIC .)

add_executable(ProcMapsTest ProcMapsTest.cpp)
target_link_libraries(ProcMapsTest Scimitar)

add_executable(ExecutableTest ExecutableTest.cpp)
target_link_libraries(ExecutableTest Scimitar)

add_executable(ScimitarTest ScimitarTest.cpp)
target_link_libraries(ScimitarTest Scimitar)
