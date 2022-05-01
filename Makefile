EXECUTABLES := $(wildcard *.c)

all:
	gcc -o demon $(EXECUTABLES)

install:
	cp demon /usr/bin

uninstall:
	rm /usr/bin/demon

clean:
	rm demon

