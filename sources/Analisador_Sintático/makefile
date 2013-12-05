all: sintatico
sintatico: main.o AnalisadorLexico.o sintatico.o arvoregen.o
	gcc -o sintatico main.o AnalisadorLexico.o sintatico.o arvoregen.o
main.o: main.c
	gcc -o main.o -c main.c
AnalisadorLexico.o: ../Analisador_Léxico/AnalisadorLexico.c ../Analisador_Léxico/AnalisadorLexico.h
	gcc -o AnalisadorLexico.o -c ../Analisador_Léxico/AnalisadorLexico.c
sintatico.o: sintatico.c sintatico.h
	gcc -o sintatico.o -c sintatico.c
arvoregen.o: arvoregen.c arvoregen.h
	gcc -o arvoregen.o -c arvoregen.c

test: sintatico
	./sintatico ../Analisador_Léxico/programa_na_linguagem_karla.k > arvore.out
