#ifndef ARVOREGEN_H_INCLUDED
#define ARVOREGEN_H_INCLUDED

struct arvgen {
char* info;
struct arvgen *prim;
struct arvgen *prox;
};
typedef struct arvgen ArvGen;
ArvGen* cria (char *c);
void insere (ArvGen* pai, ArvGen* filho);
//void imprime (ArvGen* a);

#endif // ARVOREGEN_H_INCLUDED
