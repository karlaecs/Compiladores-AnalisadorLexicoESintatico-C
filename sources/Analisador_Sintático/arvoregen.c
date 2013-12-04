#include"arvoregen.h"
#include<stdio.h>
#include<stdlib.h>

ArvoreGen* criar(char *c)
{
    ArvoreGen *a  = (ArvoreGen*)malloc(sizeof(ArvoreGen));
    a->info = c;
    a->prim = NULL;
    a->prox = NULL;
    return a;
}

void inserir(ArvoreGen* filho, ArvoreGen* pai)
{
    filho->prox = pai->prim;
    pai->prim = filho;
}

void imprime (ArvoreGen* a)
{


    if(a != NULL) {
        ArvoreGen* p = a;
        printf("%s\t",p->info);
		imprime(p->prox);
		printf("\n");
		imprime(p->prim);

    }

}






