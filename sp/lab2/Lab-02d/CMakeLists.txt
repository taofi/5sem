cmake_minimum_required(VERSION 3.10)
project(Lab-02d C)

set(CMAKE_C_STANDARD 11)

add_executable(Lab-02d Lab-02d.c)

target_compile_definitions(Lab-02d PRIVATE 
    _UNICODE 
    UNICODE 
    _CRT_SECURE_NO_WARNINGS
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(Lab-02d PRIVATE 
        -municode 
        -finput-charset=UTF-8
    )
endif()


target_link_libraries(Lab-02d PRIVATE 
    kernel32
    user32
)