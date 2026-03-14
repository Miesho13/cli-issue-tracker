CC=clang
OUT=./issue
FLAGS= -Wall -Wextra -O0 -g3
SRC=src/main.c
SRC+= src/file_helper.c
SRC+= src/sb.c
SRC+= src/issue.c
SRC+= src/commands.c

all:
	${CC} ${SRC} -o ${OUT} ${FLAGS}
