#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *pFile, *arq;

void iniciar_tex() {
    fprintf(arq,"\\documentstyle[treetex]{article}\n");
    fprintf(arq,"\\begin{document}\n");
    fprintf(arq,"\\begin{center}\n");
    fprintf(arq,"\\begin{tree}{arvore}\n");
}

void fim_tex() {
    fprintf(arq,"\\end{tree}\n");
    fprintf(arq,"\\end{center}\n");
    fprintf(arq,"\\end{document}\n");
}

int main() {
    char pai [80], filho [80];
    pFile = fopen("arvore.out", "rb");
    if(pFile == NULL) {
        puts("Falha ao abrir arvore.out");
    }

    arq = fopen("arvore.tex", "w");
    if(arq == NULL) {
        puts("Falha ao abrir arvore.out");
    }
    int cont = -1;
    iniciar_tex();

    int a = fscanf (pFile, "%s", pai);
    a = fscanf (pFile, "%s", filho);
    fprintf(arq,"\\rootnode{%s}\n", filho);
    while(a != -1) {
        cont++;
        if(cont%2 == 0) {
            a = fscanf (pFile, "%s", pai);
        }else {
            a = fscanf (pFile, "%s", filho);
            fprintf(arq,"\\treenode{%s}{%s}{%s}\n", filho, pai, filho);
        }
    }

   fim_tex();
   fclose (pFile);

}
