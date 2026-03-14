CC=clang
OUT=./issue
FLAGS= -Wall -Wextra -O0 -g3
SRC=src/main.c
SRC+= src/file_helper.c
SRC+= src/sb.c
SRC+= src/issue.c
SRC+= src/commands.c

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

all:
	${CC} ${SRC} -o ${OUT} ${FLAGS}

install: all
	install -d "$(DESTDIR)$(BINDIR)"
	install -m 0755 "$(OUT)" "$(DESTDIR)$(BINDIR)/issue"

uninstall:
	rm -f "$(DESTDIR)$(BINDIR)/issue"

clean:
	rm -f "$(OUT)"

.PHONY: all install uninstall clean
