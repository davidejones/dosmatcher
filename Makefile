CFLAGS  = -std=gnu99 -Wall -Wextra -Os -nostdlib -m32 -march=i386 \
  -Wno-unused-function \
  -ffreestanding -fomit-frame-pointer -fwrapv -fno-strict-aliasing \
  -fno-leading-underscore \
  -Wl,--nmagic,-static,-Tcom.ld
all:
	gcc $(CFLAGS) -o cmake-build-debug/dosmat.o dosmat.c *.h
	objcopy -O binary cmake-build-debug/dosmat.o cmake-build-debug/dosmat.com