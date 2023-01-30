CFLAGS=-lncurses -Wall -Wextra -g -fsanitize=address
PROG=main
SRCS=${wildcard main.c src/*.c src/**/*.c}
OBJS=${patsubst %.c,build/%.o,${SRCS}}
DEPS=${patsubst %.o,%.d,${OBJS}}

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${PROG}

build/%.o: %.c
	mkdir -p ${dir $@}
	${CC} ${CFLAGS} ${CDEFINES} -MMD -c $< -o $@

-include ${DEPS}

clean:
	rm -rf ${PROG} build

count:
	find . -name '*.c' -or -name '*.h' | xargs wc | sort -rn
