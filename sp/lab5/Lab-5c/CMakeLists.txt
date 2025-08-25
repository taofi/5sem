cmake_minimum_required(VERSION 3.10)
project(Lab-05c C)

# Устанавливаем стандарт C для проекта
set(CMAKE_C_STANDARD 11)

# Добавляем исполняемый файл
add_executable(Lab-05c Lab-05c.c)

# Компиляция с определенными макросами
target_compile_definitions(Lab-05c PRIVATE 
    _UNICODE               # Для работы с Unicode строками
    UNICODE                # Для работы с Unicode строками
    _CRT_SECURE_NO_WARNINGS # Отключение предупреждений о небезопасных функциях CRT
)

# Для компилятора Clang настраиваем дополнительные параметры
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(Lab-05c PRIVATE 
        -municode            # Включаем поддержку Unicode
        -finput-charset=UTF-8 # Устанавливаем кодировку входных файлов как UTF-8
    )
endif()

# Линковка с библиотеками Windows (Kernel и User32 для работы с WinAPI)
target_link_libraries(Lab-05c PRIVATE 
    kernel32   # Библиотека ядра Windows
    user32     # Библиотека пользовательского интерфейса Windows
)
