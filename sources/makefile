all: lexico
lexico: main.o AnalisadorLexico.o
	gcc -o lexico main.o AnalisadorLexico.o
main.o: main.c
	gcc -o main.o -c main.c
AnalisadorLexico.o: AnalisadorLexico.c AnalisadorLexico.h
	gcc -o AnalisadorLexico.o -c AnalisadorLexico.c
