#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sintatico.h"
#include "../Analisador_Léxico/AnalisadorLexico.h"
#define INTMAX 35

Token *token;
FILE *arq;
int nao_terminais[INTMAX];
int terminais[INTMAX];

/*
A chamada para a principal função, a partir dela uma variavel global token que é auxiliar, será alocada e ficará
recebendo os tokens analisados pelo Lexico,eles serão atualizados conforme o sintatico for solicitando.
*/
void analisador_sintatico() {

    arq = fopen("../tree_tex/arvore.tex", "w");
    if(arq == NULL) {
        puts("Falha ao abrir arvore.tex");
    }
    memset(nao_terminais, 0, sizeof(nao_terminais));
    memset(terminais, 0, sizeof(terminais));
    iniciar_tex();
    token = (Token*) malloc(sizeof(Token));
    token = proximo_token();
    programa();
    fim_tex();
    fclose(arq);

}

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

int corpo() {
    if(token != NULL) {
        if(token->categoria == tk_open_key) {
            fprintf(arq,"\\treenode{%dopenkey}{%dcorpo}{\\{}\n", terminais[1]++, nao_terminais[0]);
            token = proximo_token();
            fprintf(arq,"\\treenode{%dcmd}{%dcorpo}{cmd}\n", nao_terminais[6], nao_terminais[0]);
            if(cmd()) {
                nao_terminais[6]++;
                if(token->categoria == tk_close_key) {
                    fprintf(arq,"\\treenode{%dclosekey}{%dcorpo}{\\}}\n", terminais[2]++, nao_terminais[0]);
                    token = proximo_token();
                    return 1;
                }
                puts("ERRO FALTA }");
                return 0;
            }
            return 0;
        }
        puts("ERRO FALTA {");
        return 0;
    }
    return 0;
}


int programa() {
    fprintf(arq,"\\rootnode{Programa}\n");
    if(termo_program()) {
        fprintf(arq,"\\treenode{termoprogram}{root}{termo program}\n");
        if(token->categoria == tk_kw_main) {
	     fprintf(arq,"\\treenode{main}{root}{main}\n");
             token = proximo_token();
             if(corpo()) {
                fprintf(arq,"\\treenode{%dcorpo}{root}{corpo}\n", nao_terminais[0]);
                nao_terminais[0]++;
                puts("Executado com Sucesso!");
                return 1;
             }
	     exit(EXIT_FAILURE);
         }
         exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
}


int termo_program() {

    if(list_func()) {
        fprintf(arq,"\\treenode{%dlistfunc}{termoprogram}{list func}\n", nao_terminais[3]);
        nao_terminais[3]++;
        return 1;
    }
    fprintf(arq,"\\treenode{%dvazio}{termoprogram}{vazio}\n", terminais[29]++);
    //pensar como tratar esse erro dele escrever as duas producoes
    return 1;
}

int list_func() {
    if(token != NULL) {
        if(token->categoria == tk_kw_func) {
            fprintf(arq,"\\treenode{%dfunc}{%dlistfunc}{function}\n", terminais[3]++, nao_terminais[3]);
            token = proximo_token();
            if(token->categoria == tk_id) {
                fprintf(arq,"\\treenode{%did}{%dlistfunc}{id}\n", terminais[4]++, nao_terminais[3]);
                token = proximo_token();
                if(token->categoria == tk_open_par) {
                    fprintf(arq,"\\treenode{%dopenpar}{%dlistfunc}{\\(}\n", terminais[5]++, nao_terminais[3]);
                    token = proximo_token();
                    if(list_param()) {
                        fprintf(arq,"\\treenode{%dlistparam}{%dlistfunc}{list param}\n", nao_terminais[4]++, nao_terminais[3]);
                        if(token->categoria == tk_close_par) {
                            fprintf(arq,"\\treenode{%dclosepar}{%dlistfunc}{\\)}\n", terminais[6]++, nao_terminais[3]);
                            token = proximo_token();
                            if(token->categoria == tk_open_key) {
                                fprintf(arq,"\\treenode{%dopenkey}{%dlistfunc}{\\{}\n", terminais[1]++, nao_terminais[3]);
                                token = proximo_token();
                                if(cmd()){
                                    fprintf(arq,"\\treenode{%dcmd}{%dlistfunc}{cmd}", nao_terminais[6]++, nao_terminais[3]);
                                    if(token->categoria == tk_kw_return) {
                                        fprintf(arq,"\\treenode{%dreturn}{%dlistfunc}{return}\n", terminais[7]++, nao_terminais[3]);
                                        token = proximo_token();
                                        if(exp_bool()) {
                                            fprintf(arq,"\\treenode{%dexpbool}{%dlistfunc}{exp bool}\n", nao_terminais[15]++, nao_terminais[3]);
                                            if(token->categoria == tk_semicolon) {
                                                fprintf(arq,"\\treenode{%dsemicolon}{%dlistfunc}{;}\n", terminais[14]++, nao_terminais[3]);
                                                token = proximo_token();
                                                if(token->categoria == tk_close_key) {
                                                    fprintf(arq,"\\treenode{%dclosekey}{%dlistfunc}{\\}}\n", terminais[2]++, nao_terminais[3]);
                                                    token = proximo_token();
                                                    if(list_func()){
                                                        fprintf(arq,"\\treenode{%dlistfunc}{%dlistfunc}{list func}\n", nao_terminais[3]+1, nao_terminais[3]);
                                                        nao_terminais[3]++;
                                                        return 1;
                                                    }
                                                    return 0;
                                                }
                                                puts("ERRO - Falta }");
                                                return 0;
                                            }
                                            puts("ERRO - Falta ;3");
                                            return 0;
                                        }
                                        return 0;
                                    }
                                    return 0;
                                }
                                return 0;
                            }
                            puts("ERRO - Falta {");
                            return 0;
                        }
                        puts("ERRO - Falta )");
                        return 0;
                    }
                    return 0;
                }
                puts("ERRO - Falta (");
                return 0;
            }
            return 0;
        }
        fprintf(arq,"\\treenode{%dvazio}{%dlistfunc}{vazio}\n", terminais[29]++, nao_terminais[3]);
        return 1;
    }
}


int list_param() {
    if(type()) {
        fprintf(arq,"\\treenode{%dtype}{%dlistparam}{type}\n", nao_terminais[9]++, nao_terminais[4]);
        if(token->categoria == tk_id) {
            fprintf(arq,"\\treenode{%did}{%dlistparam}{id}\n", terminais[4]++, nao_terminais[4]);
            token = proximo_token();
            if(param()) {
                fprintf(arq,"\\treenode{%dparam}{%dlistparam}{param}\n", nao_terminais[5]++, nao_terminais[4]);
                nao_terminais[4]++;
                return 1;
            }
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
}


int param() {
    if(token != NULL){
        if(token->categoria == tk_comma) {
            fprintf(arq,"\\treenode{%dcomma}{%dparam}{\\,}\n", terminais[8]++, nao_terminais[5]);
            token = proximo_token();

            if(list_param()) {
                fprintf(arq,"\\treenode{%dlistparam}{%dparam}{list param}\n", nao_terminais[4]++, nao_terminais[5]);
                nao_terminais[5]++;
                return 1;
            }
            exit(EXIT_FAILURE);
        }
	exit(EXIT_FAILURE);
    }
    fprintf(arq,"\\treenode{%dvazio}{%dparam}{vazio}\n", terminais[29]++, nao_terminais[5]);
    return 1;
}


int cmd() {
    if(var_dec()) {
        fprintf(arq,"\\treenode{%dvardec}{%dcmd}{var dec}\n", nao_terminais[4]++, nao_terminais[6]);
        if(cmd()) {
            fprintf(arq,"\\treenode{%dcmd}{%dcmd}{cmd}\n", nao_terminais[6]+1, nao_terminais[6]);
            nao_terminais[6]++;
            return 1;
        }
        return 0;
    }

    else if(cond()) {
        if(cmd()) {
            return 1;
        }
        exit(EXIT_FAILURE);
    }
    else if(iter()) {
        if(cmd()) {
            return 1;
        }
        exit(EXIT_FAILURE);
    }
    else if(atrib()) {
        if(token->categoria == tk_semicolon) {
            token = proximo_token();
            if(cmd()) {
                return 1;
            }
            exit(EXIT_FAILURE);
        }
        puts("ERRO - FALTA ;");
        exit(EXIT_FAILURE);
    }
    /*else if(ch_func()) {
        if(cmd()) {
            return 1;
        }
        exit(EXIT_FAILURE);
    }*/
    fprintf(arq,"\\treenode{%dvazio}{%dcmd}{vazio}\n", terminais[29]++, nao_terminais[6]);
    return 1;
}

/*
int ch_func() {
if(saida())
return 1;
else if(entrada())
return 1;
else if(concaternar())
return 1;
return 0;
}

int saida() {
if(token->categoria == tk_kw_output) {
token = proximo_token();
if(token->categoria == tk_open_par) {
token = proximo_token();
if(token->categoria == tk_const_lit) {
token = proximo_token();
return 1;
}
else if(types())
return 1;
return 0;
}
return 0;
}
return 0;
}*/

// contador6
int var_dec()
{
    if(type()) {
        fprintf(arq,"\\treenode{%dtype}{%dvardec}{type}\n", nao_terminais[9], nao_terminais[7]);
        if(token->categoria == tk_id) {
            fprintf(arq,"\\treenode{%did}{%dvardec}{id}\n", terminais[4]++, nao_terminais[7]);
            token = proximo_token();
            if(termo_var_dec()) {
                fprintf(arq,"\\treenode{%dtermovardec}{%dvardec}{termo var dec}\n", nao_terminais[8], nao_terminais[7]);
                if(token->categoria == tk_semicolon) {
                    fprintf(arq,"\\treenode{%dsemicolon}{%dvardec}{\\;}\n", terminais[14]++, nao_terminais[7]);
                    token = proximo_token();
                    nao_terminais[7]++;
                    return 1;
                }
                puts("ERRO FALTA ;");
                return 0;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}


int termo_var_dec()
{
    if(token != NULL){
        if(token->categoria == tk_open_bra) {
            fprintf(arq,"\\treenode{%dopenbra}{%dtermovardec}{\\[}\n", terminais[12]++, nao_terminais[8]);
            token = proximo_token();
            if(token->categoria == tk_const_int) {
                fprintf(arq,"\\treenode{%dconstint}{%dtermovardec}{cte int}\n", terminais[30]++, nao_terminais[8]);
                token = proximo_token();
                if(token->categoria == tk_close_bra) {
                    fprintf(arq,"\\treenode{%dclose_bra}{%dtermovardec}{\\]}\n", terminais[13]++, nao_terminais[8]);
                    nao_terminais[8]++;
                    return 1;
                }
                return 0;
            }
            return 0;
        }
        fprintf(arq,"\\treenode{%dvazio}{%dtermovardec}{vazio}\n", terminais[29]++, nao_terminais[8]);
        nao_terminais[8]++;
        return 1;
    }
    return 0;
}


int type() {

    if(token != NULL){
        if(token->categoria == tk_kw_int || token->categoria == tk_kw_float || token->categoria == tk_kw_char) {
            fprintf(arq,"\\treenode{%dtipo}{%dtype}{%s}\n", terminais[32]++, nao_terminais[9], token->valor);
            token = proximo_token();
            nao_terminais[9]++;
            return 1;
        }
        return 0;
    }
    return 0;
}

int cond() {

    if(token != NULL){
        if(token->categoria == tk_kw_if) {
            token = proximo_token();
            if(token->categoria == tk_open_par) {
                token = proximo_token();
                if(exp_bool()) {
                    if(token->categoria == tk_close_par) {
                        token = proximo_token();
                        if(corpo()){
                            if(termo_cond())
                                return 1;
                            return 0;
                        }
                        return 0;
                    }
                    puts("ERRO - Falta )");
                    exit(EXIT_FAILURE);
                }
                return 0;
            }
            puts("ERRO - Falta (");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

int termo_cond() {

    if(token != NULL){
        if(token->categoria == tk_kw_else) {
            token = proximo_token();
            if(fator_cond())
                return 1;
            return 0;
        }
        return 2;
    }
}

int fator_cond(){

    if(token != NULL){
        if(corpo())
            return 1;
        else if(cond())
            return 1;
        return 0;
    }
    return 0;
}

int iter() {

    if(token != NULL){
        if(token->categoria == tk_kw_for) {
            token = proximo_token();
            if(token->categoria == tk_open_par) {
                token = proximo_token();
                if(atrib()) {
                    if(token->categoria == tk_semicolon) {
                        token = proximo_token();
                        if(exp_bool()) {
                            if(token->categoria == tk_semicolon) {
                                token = proximo_token();
                                if(atrib()) {
                                    if(token->categoria == tk_close_par) {
                                        token = proximo_token();
                                        if(corpo())
                                            return 1;
                                        return 0;
                                    }
                                    puts("ERRO - Falta )");
                                    exit(EXIT_FAILURE);
                                }
                                return 0;
                            }
                            puts("ERRO - Falta ;");
                            exit(EXIT_FAILURE);
                        }
                        return 0;
                    }
                    puts("ERRO - Falta ;");
                    exit(EXIT_FAILURE);
                }
                return 0;
            }
            puts("ERRO - Falta (");
            exit(EXIT_FAILURE);
        }

        else if(token->categoria == tk_kw_while) {
            token = proximo_token();
            if(token->categoria == tk_open_par) {
                token = proximo_token();
                if(exp_bool()) {
                    if(token->categoria == tk_close_par) {
                        token = proximo_token();
                        if(corpo())
                            return 1;
                        return 0;
                    }
                    puts("ERRO - Falta )");
                    exit(EXIT_FAILURE);
                }
                return 0;
            }
            puts("ERRO - Falta (");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

int atrib() {

    if(token != NULL)
    {
        if(token->categoria == tk_id) {
            token = proximo_token();
            if(token->categoria == tk_op_atrib) {
                token = proximo_token();
                if(exp())
                    return 1;
                return 0;
            }
            return 0;
        }
    }
    return 0;
}

int exp() {

    if(exp_bool())
        return 1;
    else if(exp_arit())
        return 1;
    return 0;
}

int exp_bool() {
    if(termo_bool()){
        if(A())
            return 1;
        return 0;
    }
    return 0;
}

int A() {
    if(token != NULL){
         if(token->categoria == tk_kw_or) {
            token = proximo_token();
            if(termo_bool()) {
                if(B())
                    return 1;
                return 0;
            }
            return 0;
         }
         return 2;
    }
     return 0;
}

int termo_bool() {
    if(fator_bool()){
        if(B())
            return 1;
        return 0;
    }
    return 0;
}

int B() {
    if(token != NULL)
    {
        if(token->categoria == tk_kw_and) {
            token = proximo_token();
            if(fator_bool()){
                if(B())
                    return 1;
                return 0;
            }
            return 0;
        }
        return 2;
    }
    return 0;
}

int fator_bool() {
    if(token != NULL){
        if(token->categoria == tk_kw_not) {
            token = proximo_token();
            if(fator_bool())
                return 1;
            return 0;
        }

    else if(exp_rel())
        return 1;
    }
    return 0;
}

int exp_rel() {
    if(exp_arit()) {
        if(C())
            return 1;
        return 0;
    }
    return 0;
}

int C()
{
    if(token != NULL)
    {
        if(token->categoria == tk_rel_op) {
            token = proximo_token();
            if(exp_arit())
                return 1;
            return 0;
        }
    }
    return 2;
}

int exp_arit() {
    if(termo_arit()){
         if(D())
            return 1;
         return 0;
    }
    return 0;
}

int D() {
    if(token != NULL){
        if(token->categoria == tk_op_add) {
            token = proximo_token();
            if(termo_arit()) {
                if(D())
                    return 1;
                return 0;
            }
            return 0;
        }
        else if(token->categoria == tk_op_sub) {
            token = proximo_token();
            if(termo_arit()) {
                if(D())
                    return 1;
                return 0;
            }
            return 0;
        }
        return 2;
    }
    return 0;
}

int termo_arit() {
    if(fator_arit()) {
        if(E())
            return 1;
        return 0;
    }
    return 0;
}

int E() {
    if(token != NULL){
        if(token->categoria == tk_op_mul){
            token = proximo_token();
            if(fator_arit()){
                if(E())
                    return 1;
            }
            return 0;
        }
        else if(token->categoria == tk_op_div) {
            token = proximo_token();
            if(fator_arit()){
                if(E())
                    return 1;
            }
            return 0;
        }
        return 2;
    }
    return 0;
}

int fator_arit() {

    if(token != NULL){
        if(token->categoria == tk_const_int || token->categoria == tk_const_float || token->categoria == tk_id){
            token = proximo_token();
            return 1;
        }
    }
    return 0;
}


