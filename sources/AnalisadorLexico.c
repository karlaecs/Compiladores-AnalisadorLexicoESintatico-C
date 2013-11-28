#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "AnalisadorLexico.h"
#define MAX 50

// Variaveis globais
FILE *arquivo;
size_t numero_bytes=100;
int bytes_lidos=0, linha1=0, coluna1=0, i=0, j=0, primeiro=0, comentario = 0, coluna2 = 0;
char *linha_do_arquivo;

// Enumeracao das Categorias.

enum categoria{

    tk_rel_op = 256,
    tk_id = 257,
    tk_const_int = 258,
    tk_const_float =259,
    tk_const_lit = 260,
    tk_kw_int = 261,
    tk_kw_float = 262,
    tk_kw_char = 263,
    tk_kw_if = 264,
    tk_kw_else = 265,
    tk_kw_for = 266,
    tk_kw_while = 267,
    tk_kw_break = 268,
    tk_kw_not = 269,
    tk_kw_and = 270,
    tk_kw_or = 271,
    tk_kw_main = 272,
    tk_kw_func = 273,
    tk_kw_return = 274,
    tk_kw_strcmp = 275,
    tk_kw_strcop = 276,
    tk_kw_input = 277,
    tk_kw_output = 278,
    tk_op_add = 279,
    tk_op_sub = 280,
    tk_op_mul = 281,
    tk_op_div = 282,
    tk_op_atrib = 283,
    tk_open_par = 284,
    tk_close_par = 285,
    tk_open_bra = 286,
    tk_close_bra = 287,
    tk_close_key = 288,
    tk_open_key = 289,
    tk_semicolon = 290,
    tk_comma = 291,
    erro  = 292
};


struct token{
    char *valor;
    Categoria categoria;
    int linha;
    int coluna;
};


// Funcao utilizada para imprimir token

void imprimir_token(Token *token){

	if(token != NULL)
    printf("[Categoria: %d Valor: %s Linha: %d Coluna: %d]\n",
(int)token->categoria, token->valor, token->linha, token->coluna);
}

//Vetor de ponteiro para funcoes

Token* (*func[128])() = {
    &proximo_token, &proximo_token, &proximo_token, 
    &proximo_token, &proximo_token, &proximo_token,//0 - 5
    &proximo_token, &proximo_token, &proximo_token, &proximo_token, &proximo_token, //6-10
    &proximo_token, &proximo_token, &proximo_token, &proximo_token, &proximo_token, //11 - 15
    &proximo_token, &proximo_token, &proximo_token, &proximo_token, &proximo_token, // 16 - 20
    &proximo_token, &proximo_token, &proximo_token, &proximo_token, &proximo_token, //21 - 25
    &proximo_token, &proximo_token, &proximo_token, &proximo_token, &proximo_token, //26- 30
    &proximo_token, &proximo_token, &operador_relacional,
    &constante_literal, &indefinidos,//31 - 35
    &indefinidos, &indefinidos, &indefinidos, &indefinidos, &delimitador, // 36 - 40
    &delimitador, &operador_aritmetico, &operador_aritmetico,
    &delimitador, &operador_aritmetico, // 41 - 45
    &indefinidos, &operador_aritmetico, &constante_numerica, 
    &constante_numerica, &constante_numerica, // 46 - 50
    &constante_numerica, &constante_numerica, &constante_numerica, 
    &constante_numerica, &constante_numerica, // 51 - 55
    &constante_numerica, &constante_numerica,  &indefinidos, 
    &delimitador, &operador_relacional, // 56 - 60
    &operador_relacional, &operador_relacional, 
    &delimitador, &indefinidos, &palavra, // 61 - 65
    &palavra, &palavra, &palavra, &palavra, &palavra, 
    &palavra, &palavra, &palavra, &palavra, &palavra, // 66 - 75
    &palavra, &palavra, &palavra, &palavra, &palavra,&palavra, &palavra,
    &palavra, &palavra, &palavra, // 76 - 85
    &palavra, &palavra, &palavra, &palavra, &palavra, // 86 - 90
    &delimitador, &indefinidos, &delimitador, &indefinidos, &indefinidos, // 91 - 95
    &indefinidos, &palavra, &palavra, &palavra, &palavra, 
    &palavra, &palavra, &palavra, &palavra, &palavra, // 96 - 105
    &palavra, &palavra, &palavra, &palavra, &palavra, &palavra, 
    &palavra, &palavra, &palavra, &palavra, // 106 - 115
    &palavra, &palavra, &palavra, &palavra, &palavra, &palavra, 
    &palavra, &delimitador, &indefinidos, &delimitador, // 116 - 125
    &indefinidos, &indefinidos // 126 e 127
};

// Funcoes que analisam os caracteres passados de acordo com sua posicao na tabela ASCII

Token* indefinidos(){
    printf("Erro Linha:%d Coluna:%d \nCaractere %c não permitido na linguagem\n",
 linha1, coluna1, linha_do_arquivo[i]);
    return proximo_token();
}

Token* delimitador(){
    Token *token = (Token*) malloc(sizeof(Token));
    token->valor=(char*)malloc(3*sizeof(char));
    token->linha = linha1;
    token->coluna = coluna1;
    switch(linha_do_arquivo[i]){
        case '(':
            token->categoria = tk_open_par;
            strcpy(token->valor,"(");
            break;

        case ')':
            token->categoria = tk_close_par;
            strcpy(token->valor,")");
            break;

        case '{':
            token->categoria = tk_open_key;
            strcpy(token->valor,"{");
            break;

        case '}':
            token->categoria = tk_close_key;
            strcpy(token->valor,"}");
            break;

          case '[':
            token->categoria = tk_open_bra;
            strcpy(token->valor,"[");
            break;

        case ']':
            token->categoria = tk_close_bra;
            strcpy(token->valor,"]");
            break;
        case ';':
            token->categoria = tk_semicolon;
            strcpy(token->valor,";");
            break;
        case ',':
            token->categoria = tk_comma;
            strcpy(token->valor,",");
            break;
   }
   return token;
}

Token* operador_aritmetico(){
    Token *token = (Token*) malloc(sizeof(Token));
    token->valor=(char*)malloc(3*sizeof(char));
    token->linha = linha1;
    token->coluna = coluna1;
    switch(linha_do_arquivo[i]){
        case '+':
            token->categoria = tk_op_add;
            strcpy(token->valor,"+");
            break;

        case '-':
            token->categoria = tk_op_sub;
            strcpy(token->valor,"-");
            break;

        case '*':
            i++;
            if(linha_do_arquivo[i] == '/'){
                printf("Erro na Linha: %d e Colunha: %d\n", linha1, coluna1);
                puts("/* esperado");
                return proximo_token();
            }
            else{
                i--;
                token->categoria = tk_op_mul;
                strcpy(token->valor,"*");
            }
            break;

        case '/':
            i++;
            if(linha_do_arquivo[i] == '*'){
              	eliminar_comentario();
              	return proximo_token();
            }else{
                i--;
                token->categoria = tk_op_div;
                strcpy(token->valor,"/");
            }
            break;
    }

    return token;
}

Token* operador_relacional(){
    Token *token = (Token*) malloc(sizeof(Token));
    token->valor=(char*)malloc(3*sizeof(char));
    token->categoria = tk_rel_op;
    token->linha = linha1;
    token->coluna = coluna1;

    switch(linha_do_arquivo[i]){
        case '<':
            i++;
            if(linha_do_arquivo[i] == '='){
                strcpy(token->valor,"<=");
            }else{
              i--;
              strcpy(token->valor,"<");
            }
            break;

         case '>':
            i++;
            if(linha_do_arquivo[i] == '='){
                strcpy(token->valor,">=");
            }else{
              i--;
              strcpy(token->valor,">");
            }
            break;

        case '!':
            i++;
            if(linha_do_arquivo[i] == '='){
                strcpy(token->valor,"!=");
            }else{
              i--;
              puts("Palavra não permitida pela linguagem");
            }
            break;

        case '=':
            i++;
            if(linha_do_arquivo[i] == '='){
                strcpy(token->valor,"==");
            }else{
              i--;
              token->categoria = tk_op_atrib;
              strcpy(token->valor,"=");
            }
            break;
    }

    return token;
}

Categoria achar_categoria(char *buffer){
    if(!strcmp(buffer, "int"))
        return tk_kw_int;
    if((!strcmp(buffer, "float")))
        return tk_kw_float;
    if((!strcmp(buffer, "char")))
        return tk_kw_char;
    if((!strcmp(buffer, "if")))
        return tk_kw_if;
    if((!strcmp(buffer, "else")))
        return tk_kw_else;
    if((!strcmp(buffer, "for")))
        return tk_kw_for;
    if((!strcmp(buffer, "while")))
        return tk_kw_while;
    if((!strcmp(buffer, "break")))
        return tk_kw_break;
    if((!strcmp(buffer, "not")))
        return tk_kw_not;
    if((!strcmp(buffer, "and")))
        return tk_kw_and;
    if((!strcmp(buffer, "or")))
        return tk_kw_or;
    if((!strcmp(buffer, "main")))
        return tk_kw_main;
    if((!strcmp(buffer, "function")))
        return tk_kw_func;
    if((!strcmp(buffer, "return")))
        return tk_kw_return;
    if((!strcmp(buffer, "strcmp")))
        return tk_kw_strcmp;
    if((!strcmp(buffer, "strcop")))
        return tk_kw_strcop;
    if((!strcmp(buffer, "input")))
        return tk_kw_input;
    if((!strcmp(buffer, "output")))
        return tk_kw_output;
    return erro;
}

Token* palavra(){
    Token *token = (Token*) malloc(sizeof(Token));
    token->valor=(char*)malloc(3*sizeof(char));

    char *palavra = (char *)malloc(MAX*sizeof(char));
    token->linha = linha1;
    token->coluna = coluna1;

    while(isalpha(linha_do_arquivo[i]) || isdigit(linha_do_arquivo[i]) ||
            linha_do_arquivo[i]== '_'){
        if(j > MAX-10){
            palavra = (char*)realloc(palavra, (MAX+50)*sizeof(char));
        }
        palavra[j] = tolower(linha_do_arquivo[i]);
        j++, i++, coluna2++;
    }
    palavra[j] = '\0';
    i--;
    coluna2--;


    if(achar_categoria(palavra) != erro)
    {
        strcpy(token->valor, palavra);
        token->categoria = achar_categoria(palavra);
    }
    else{
        token->categoria = tk_id;
        strcpy(token->valor, palavra);
    }
    coluna1 += coluna2;
    coluna2 = 0, j = 0;
	return token;
}

Token* constante_numerica(){
    int ponto_flutuante = 0, k = 0, erro = 0;
    char *constante = (char *)malloc(MAX*sizeof(char));
    Token *token = (Token*) malloc(sizeof(Token));
    token->valor=(char*)malloc(3*sizeof(char));
    token->linha = linha1;
    token->coluna = coluna1;

    while(isdigit(linha_do_arquivo[i]) || linha_do_arquivo[i] == '.'){
        if(isdigit(linha_do_arquivo[i])){
            constante[k] = linha_do_arquivo[i];
            i++, k++, erro = 0;
        }
        else if(linha_do_arquivo[i] == '.' && ponto_flutuante == 0){
            constante[k] = linha_do_arquivo[i];
            ponto_flutuante = 1, i++, k++, erro = 0;
        }else{
            printf("Erro na Linha: %d e Colunha: %d\n", linha1, coluna1);
            puts("Palavra não permitida pela linguagem");
            erro = 1;
            return proximo_token();
        }
    }
    if(erro == 0)
    {
        constante[k] = '\0';
        i--, erro = 1;
        if(ponto_flutuante){
            token->categoria = tk_const_float;
            strcpy(token->valor, constante);
        }else{
            token->categoria = tk_const_int;
            strcpy(token->valor, constante);
        }
    }
    return token;
}

Token* constante_literal(){

    char *buffer = (char *)malloc(MAX*sizeof(char));
    Token *token = (Token *)malloc(sizeof(Token));
    token->linha = linha1;
    token->coluna = ++coluna1;
    token->categoria = tk_const_lit; 
    i++;

    while(1){
        buffer[j] = linha_do_arquivo[i];
        if(j > MAX-10){
            buffer= (char*)realloc(buffer, (MAX+50)*sizeof(char));
        }
        if(i == bytes_lidos-1){
            printf("Faltando caracter terminador %c\n",34);
            return proximo_token();
        }
        i++, j++, coluna2++;
        if(linha_do_arquivo[i] == '"'){
            break;
        }
   }
    buffer[j] = '\0';
    token->valor=(char*)malloc((strlen(buffer)+1)*sizeof(char));
    strcpy(token->valor, buffer);
    coluna1 += coluna2, j = 0;
    free(buffer);
    return token;
}

//Funcao para elimnar comentarios, 
//tudo que esta entre os delimitadores de comentarios sera desconsiderado

void eliminar_comentario(){
    comentario = 1;
	while( comentario == 1 && bytes_lidos != -1)
	{
        if(i == (bytes_lidos-1) ){
            i=0, coluna1 = 0;
            bytes_lidos = getline(&linha_do_arquivo, &numero_bytes, arquivo);
            linha1++, coluna1++;
        }else{
            i++, coluna1++;
        }
        if(linha_do_arquivo[i] == '*' && (i != bytes_lidos-1))	{
            if(linha_do_arquivo[i + 1] == '/'){
                i++, comentario = 0;
            }
        }
    }
    if(comentario == 1){
         printf("Erro na Linha: %d e Colunha: %d\n", linha1, coluna1);
        puts(" */ esperado");
    }
}


/*Funcao principal a partir dela que sao chamadas outras funcoes para ser feita a analise.
* A analise e' feita no modo on the fly
* Onde cada lexema do codigo e' lido, analisado e retornado.
*
*/
Token* proximo_token(){
    if(i == (bytes_lidos-1) || primeiro == 0 ){
        i=0, coluna1 = 0;
        bytes_lidos = getline(&linha_do_arquivo, &numero_bytes, arquivo);
        if(bytes_lidos == -1){
            return NULL;
        }
        linha1++, coluna1++, primeiro = 1;

    }else if(feof(arquivo)){
        puts("Fim do arquivo");
        return NULL;

    }else{
        i++;
        coluna1++;
    }
    return func[(int)linha_do_arquivo[i]]();
}

//Funcoes de inicializacao e de encerramento

void iniciar(int argc, char *argv[]){
    linha_do_arquivo = (char*) malloc (numero_bytes);
    arquivo = fopen(argv[1], "r");
    if(arquivo == NULL){
        puts("Erro ao abrir o arquivo");
    }
}

void encerrar(){
    fclose(arquivo);
}




