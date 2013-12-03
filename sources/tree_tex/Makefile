#Author: Anthony Bloesch
#Title: generate.c
#
#Copyright (C) Anthony Bloesch 1993
#
#Version: 1.0
#
#Usage:
#  make
#
#  also
#
#  make lint
#  make clean
#  make shar
#  make test

#Debugging
#DEBUG=-g
#OPTIMIZATION=

#Production
DEBUG=
OPTIMIZATION=-fast

LINT=~/Downloads/tree_tex
SHAR=~/Downloads/tree_tex
LATEX=~/Downloads/tree_tex
COMPILE=gcc ${DEBUG} -c -vc -Xc ${OPTIMIZATION}
LINK=${CC} ${DEBUG}

SOURCE1=Makefile layout.c layout.h treetex.c
SOURCE2=README treetex.1 test1.tex test2.tex treetex.sty
SOURCE=${SOURCE1} ${SOURCE2}

all: treetex

treetex: Makefile treetex.o layout.o
	gcc -o treetex treetex.o layout.o

treetex.o: Makefile treetex.c layout.h
	gcc -c -o treetex.o treetex.c

layout.o: Makefile layout.c layout.h
	gcc -c -o layout.o layout.c

#                                 Utilities

clean:
	rm -f *.o *.tli *.tlo *.pdf *.dvi *.aux *.log *.plg *.shar? *~ treetex

lint:
	${LINT} treetex.c layout.c

shar:
	${SHAR} -n1 -e2 ${SOURCE1} >treetex.shar1
	${SHAR} -n2 -e2 ${SOURCE2} >treetex.shar2

test: treetex
	${LATEX} arvore.tex
	./treetex arvore
	${LATEX} test1.tex
	./treetex testtreea
	./treetex testtreeb
	./treetex testtreec
	${LATEX} test1.tex
	${LATEX} test2.tex
	./treetex testtreed
	./treetex testtreee
	${LATEX} test2.tex
