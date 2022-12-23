.POSIX:

CFLAGS  = -Wall -Wextra -Wpedantic -Werror \
	  -O3 -march=native -mtune=native \
	  -std=c11 -pipe
LDFLAGS = -llux

PREFIX  = /usr/local
DPREFIX = ${DESTDIR}${PREFIX}
MANDIR  = ${DPREFIX}/share/man

target = lux

all: ${target}
${target}: ${target}.c

install:
	mkdir -p ${DPREFIX}/bin ${MANDIR}/man1
	cp ${target} ${DPREFIX}/bin
	cp *.1 ${MANDIR}/man1

clean:
	rm -f ${target}
