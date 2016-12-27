# Makefile
# Created by Tommie Lindberg ( c15tlg )

CC = gcc

CFLAGS += -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -g -std=c11 

OBJS = list.o

all: ${OBJS}
	${CC} ${CFLAGS} mpasswdSort.c ${OBJS} -o mpasswdsort

list.o: list.c list.h

clean:
	@rm -rf mpasswdSort *.o *.dSYM

.PHONY: all clean

