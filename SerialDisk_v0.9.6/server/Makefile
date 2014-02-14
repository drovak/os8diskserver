#
# os8diskserver
#
# server/Makefile: Makefile for server part
#
#
# Author: Philipp Hachtmann (hachti@hachti.de)



EXEC=os8disk
OBJS=server.o config.o comm.o

CFLAGS=-Wall
LDFLAGS=
CC=gcc

default : ${EXEC}

${EXEC}: ${OBJS}
	@echo LD $^
	@$(CC) -o $@ ${LDFLAGS} $^

%.o:%.c *.h
	@echo CC $<
	@$(CC) -c -o $@ ${CFLAGS} $<

clean:
	@echo CLEAN
	@rm -f ${EXEC} *.o *~


.PHONY: default clean