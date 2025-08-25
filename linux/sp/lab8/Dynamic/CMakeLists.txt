cmake_minimum_required(VERSION 3.10)
project(StudentManagement)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/Mappingd.c)
add_library(Mappingd SHARED ${SOURCE_FILES})

target_include_directories(Mappingd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(Lab-08b src/Lab-08b.c)

target_link_libraries(Lab-08b PRIVATE Mappingd)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab-08b PRIVATE Threads::Threads)