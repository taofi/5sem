cmake_minimum_required(VERSION 3.10)

project(Lab-08b)

set(SOURCE_FILES src/dllmain.c src/Lab-08b.c)

include_directories(include)

# 1
add_library(Mappingd SHARED src/dllmain.c src/Mappingd.def)

# 2
add_library(Mappingd-2 SHARED src/dllmain-2.c)

# Подключаем общий заголовочный файл
include_directories(include)

# Добавляем заголовочные файлы для обеих библиотек
target_sources(Mappingd PRIVATE include/Mappingd.h include/str.h)
target_sources(Mappingd-2 PRIVATE include/Mappingd-2.h include/str.h)

# Создаем исполняемый файл
add_executable(Lab-08b src/Lab-08b.c)

# Линкуем библиотеки с исполняемым файлом
target_link_libraries(Lab-08b Mappingd Mappingd-2)