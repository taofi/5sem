cmake_minimum_required(VERSION 3.10)
project(Service09 C)

set(CMAKE_C_STANDARD 11)

add_executable(Service09 Service09.c)



target_compile_definitions(Service09 PRIVATE 
    _UNICODE 
    UNICODE 
    _CRT_SECURE_NO_WARNINGS
)



if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(Service09 PRIVATE 
        -municode 
        -finput-charset=UTF-8
    )
endif()


target_link_libraries(Service09 PRIVATE 
    kernel32
    user32
)