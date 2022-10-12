.POSIX:

CFLAGS  = -std=c11 -O3 -pipe -Wall -Wextra -Werror -pedantic
LDFLAGS = -llux

PREFIX  = /usr
DPREFIX = ${DESTDIR}${PREFIX}
MANDIR  = ${DPREFIX}/share/man

target = lux

all: ${target}
${target}: main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

install:
	mkdir -p ${DPREFIX}/bin ${MANDIR}/man1
	cp ${target} ${DPREFIX}/bin
	cp *.1 ${MANDIR}/man1

clean:
	rm -f ${target}
