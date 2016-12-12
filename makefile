CC=gcc

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wstrict-prototypes

DEPS = list.h

OBJ = mainu.o list.o 

.PHONY: all
all: mpasswdsort

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mpasswdsort: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
