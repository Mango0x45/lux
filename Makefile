.POSIX:

LDFLAGS = -llux
CFLAGS = -std=c11 -O3 -pipe -Wall -Wextra -Werror -pedantic
PREFIX = /usr
MANDIR = ${PREFIX}/share/man

target = lux

all: ${target}
${target}: main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

install:
	mkdir -p ${PREFIX}/bin ${MANDIR}/man1
	cp ${target} ${PREFIX}/bin
	cp *.1 ${MANDIR}/man1

clean:
	rm -f ${target}
