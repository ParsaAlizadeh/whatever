PROGRAM=main
COPTIMIZE = 0
CFLAGS = -Wall -Wextra -O${COPTIMIZE} ${CDEFINES} -fsanitize=address -g3 -lncurses
SOURCES = ${wildcard main.c src/*.c src/**/*.c}
OBJECTS = ${patsubst %.c,build/%.o,${SOURCES}}
DEPENDS = ${patsubst %.o,%.d,${OBJECTS}}

all: ${PROGRAM}

${PROGRAM}: ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${PROGRAM}

build/%.o: %.c
	mkdir -p ${dir $@}
	${CC} ${CFLAGS} -MMD -c $< -o $@

-include ${DEPENDS}

clean:
	rm -rf ${PROGRAM} build

count:
	find . -name '*.c' -or -name '*.h' | xargs wc | sort -rn
