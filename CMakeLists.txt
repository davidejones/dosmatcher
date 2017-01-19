cmake_minimum_required(VERSION 3.6)
project(dosmatcher)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES dosmatcher.c)

set(CMAKE_C_FLAGS "-Wall -Werror -std=gnu99 -Os -nostdlib -m32 -march=i386 -Wno-unused-function")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing -fno-leading-underscore")
# linking
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--nmagic,-static,-T${CMAKE_CURRENT_SOURCE_DIR}/com.ld")

add_executable(dosmatcher ${SOURCE_FILES})