cmake_minimum_required(VERSION 3.5)
project(Scanner01 C)

set(CMAKE_C_STANDARD 99)

add_executable(Scanner01 main.c systemdefs.h scanner.c scanner.h token.h parser.c parser.h assmblr.h assmblr.c tables.h tables.c)
