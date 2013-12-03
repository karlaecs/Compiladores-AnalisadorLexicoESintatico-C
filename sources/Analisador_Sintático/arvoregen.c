#include"arvoregen.h"
#include<stdio.h>
#include<stdlib.h>

ArvGen* cria(char *c)
{
    ArvGen *a  = (ArvGen*)malloc(sizeof(ArvGen));
    a->info = c;
    a->prim = NULL;
    a->prox = NULL;
    return a;
}

void insere(ArvGen* pai, ArvGen* filho)
{
    filho->prox = pai->prim;
    pai->prim = filho;
}

/*void imprime (ArvGen* a)
{


    if(a != NULL) {
        ArvGen* p = a;
        printf("%s\t",p->info);
        imprime(p->prox);
        printf("\n");
        imprime(p->prim);

    }

}*/





