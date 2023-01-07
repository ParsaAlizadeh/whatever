CC=cc
CFLAGS=-Wall -Wextra
SRC=${wildcard src/*.c}
OBJ=${patsubst src/%.c,build/%.o,${SRC}}

all: ${OBJ}
	@echo ${SRC}
	${CC} ${OBJ} -o main ${CFLAGS}
.PHONY: all

build/%.o: src/%.c
	mkdir -p ${dir $@}
	${CC} -o $@ $< -c ${CFLAGS}

clean:
	rm -rf main build
.PHONY: clean