#include "../Analisador_Léxico/AnalisadorLexico.h"
#include "sintatico.h"
#include<stdlib.h>


int main(int argc, char *argv[]){

    iniciar(argc, argv);
    analisador_sintatico();

    encerrar();
    return 0;
}
