#include"arvoregen.h"
#include<stdio.h>
#include<stdlib.h>

ArvoreGen* criar(char *c)
{
    ArvoreGen *a  = (ArvoreGen*)malloc(sizeof(ArvoreGen));
    a->info = c;
    a->prim = NULL;
    a->prox = NULL;
    a->pai_nome = NULL;
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
		imprime(p->prox);
		// pai e filho
		printf("%s %s\n",p->pai_nome, p->info);
		imprime(p->prim);

    }

}






