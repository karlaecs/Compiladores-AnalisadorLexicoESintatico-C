#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H


typedef struct token Token;

typedef enum categoria Categoria;

Categoria achar_categoria(char *buffer);

void imprimir_token(Token *token);

char* pegar_valor_token(Token *token);

Token* delimitador();

Token* operador_aritmetico();

Token* operador_relacional();

Token* palavra();

Token* constante_literal();

Token* constante_numerica();

Token* indefinidos();

void iniciar(int argc, char *argv[]);

void encerrar();

Token* proximo_token();

void eliminar_comentario();


#endif
