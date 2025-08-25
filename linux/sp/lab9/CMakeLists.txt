cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER clang)
project(Lab09)


add_executable(Demon09 Demon09.c)

install(FILES /etc/demon09d/config.cfg demon09d.service
        DESTINATION /etc/systemd/system)
