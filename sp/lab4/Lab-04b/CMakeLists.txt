cmake_minimum_required(VERSION 3.10)
project(Lab-04b C)

set(CMAKE_C_STANDARD 11)

add_executable(Lab-04b Lab-04b.c)



target_compile_definitions(Lab-04b PRIVATE 
    _UNICODE 
    UNICODE 
    _CRT_SECURE_NO_WARNINGS
)



if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(Lab-04b PRIVATE 
        -municode 
        -finput-charset=UTF-8
    )
endif()


target_link_libraries(Lab-04b PRIVATE 
    kernel32
    user32
)