cmake_minimum_required(VERSION 3.6)
project(dosmatcher)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES dosmat.c)
#
#set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "")
#set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "")
#set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")


set(FLAGS "-std=gnu99 -Wall -Wextra -Os -nostdlib -m32 -march=i386 -Wno-unused-function")
set(FLAGS "${FLAGS} -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing -fno-leading-underscore")
#set(FLAGS "${FLAGS} -Wl,--nmagic,-static,-T${CMAKE_CURRENT_SOURCE_DIR}/com.ld")
set(CMAKE_C_FLAGS "${FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${FLAGS}")

add_executable(dosmatcher ${SOURCE_FILES})
#set_target_properties(dosmatcher PROPERTIES LINK_FLAGS -Wl,-nodefaultlibs,--nmagic,-static,-T${CMAKE_CURRENT_SOURCE_DIR}/com.ld)
#set_target_properties(dosmatcher PROPERTIES LINK_FLAGS "-Wl,--nmagic,-static,-T${CMAKE_CURRENT_SOURCE_DIR}/com.ld")
#set_target_properties(dosmatcher PROPERTIES LINKER_LANGUAGE C)
#target_link_libraries(dosmatcher "")

set(CMAKE_C_LINK_EXECUTABLE "C:\\MinGW\\bin\\ld.exe --nmagic -static -T ${CMAKE_CURRENT_SOURCE_DIR}/com.ld -o dosmat.com CMakeFiles\\dosmatcher.dir\\dosmatcher.c.obj")
#set(CMAKE_C_LINK_EXECUTABLE "-Wl,--nmagic,-static,-T${CMAKE_CURRENT_SOURCE_DIR}/com.ld")
#"C:\Program Files (x86)\JetBrains\CLion 2016.3\bin\cmake\bin\cmake.exe" -E remove -f CMakeFiles\dosmatcher.dir/objects.a
#C:\MinGW\bin\ar.exe cr CMakeFiles\dosmatcher.dir/objects.a @CMakeFiles\dosmatcher.dir\objects1.rsp
#C:\MinGW\bin\gcc.exe  -std=gnu99 -Wall -Wextra -Os -static-libgcc -nostdlib -nodefaultlibs -m32 -march=i386 -Wno-unused-function -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing -fno-leading-underscore -std=gnu99 -Wall -Wextra -Os -static-libgcc -nostdlib -nodefaultlibs -m32 -march=i386 -Wno-unused-function -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing -fno-leading-underscore   -Wl,--whole-archive CMakeFiles\dosmatcher.dir/objects.a -Wl,--no-whole-archive  -o dosmatcher.exe -Wl,--out-implib,libdosmatcher.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles\dosmatcher.dir\linklibs.rsp

add_custom_target(dosmatcher_makefile ALL COMMAND mingw32-make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


add_custom_command(
        OUTPUT dosmat.com DEPENDS dosmatcher
        COMMAND objcopy -O binary dosmatcher.exe dosmat.com
        COMMENT "objcopying dosmatcher.exe to dosmatcher.com"
)
add_custom_target(comfile ALL DEPENDS dosmatcher.com)