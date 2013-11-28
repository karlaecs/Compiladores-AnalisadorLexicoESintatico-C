#include "AnalisadorLexico.h"
#include<stdlib.h>


int main(int argc, char *argv[]){

    iniciar(argc, argv);
    Token *token = proximo_token();
    while(token != NULL){
        imprimir_token(token);
        token = proximo_token();
    }
    encerrar();
    return 0;
}
