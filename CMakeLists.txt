cmake_minimum_required(VERSION 3.26)
project(HTTPcServer C)

set(CMAKE_C_STANDARD 99)

add_executable(HTTPcServer main.c
        server.c
        server.h
        http_server.c
        http_server.h
        ht.c
        ht.h)

target_link_libraries(HTTPcServer ws2_32)