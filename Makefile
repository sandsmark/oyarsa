OBJECTS=boot.o main.o
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=-felf

all: $(OBJECTS) link

clean:
	-rm -f *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(OBJECTS)

.s.o:
	nasm $(ASFLAGS) $<
