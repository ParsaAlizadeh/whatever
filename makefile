CC=cc
CFLAGS=-Wall -Wextra -g
SRC=${wildcard main.c src/*.c src/commands/*.c}
OBJ=${patsubst %.c,build/%.o,${SRC}}

all: ${OBJ}
	@echo ${SRC}
	${CC} ${OBJ} -o main ${CFLAGS}
.PHONY: all

build/%.o: %.c
	mkdir -p ${dir $@}
	${CC} -o $@ $< -c ${CFLAGS}

clean:
	rm -rf main build
.PHONY: clean