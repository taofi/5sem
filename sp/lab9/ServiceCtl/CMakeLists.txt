cmake_minimum_required(VERSION 3.10)
project(ServiceCtrl C)

set(CMAKE_C_STANDARD 11)

add_executable(ServiceCtrl ServiceCtrl.c)



target_compile_definitions(ServiceCtrl PRIVATE 
    _UNICODE 
    UNICODE 
    _CRT_SECURE_NO_WARNINGS
)



if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(ServiceCtrl PRIVATE 
        -municode 
        -finput-charset=UTF-8
    )
endif()


target_link_libraries(ServiceCtrl PRIVATE 
    kernel32
    user32
)