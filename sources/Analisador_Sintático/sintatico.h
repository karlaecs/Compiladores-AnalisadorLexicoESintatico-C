#ifndef SINTATICO_H_INCLUDED
#define SINTATICO_H_INCLUDED

#include "arvoregen.h"
#include "../Analisador_Léxico/AnalisadorLexico.h"


void analisador_sintatico();
int corpo(ArvoreGen *pai);
int programa();
int termo_program(ArvoreGen *pai);
int cmd(ArvoreGen *pai);
int var_dec(ArvoreGen *pai);
int termo_var_dec(ArvoreGen *pai);
int type(ArvoreGen *pai);
int cond(ArvoreGen *pai);
int termo_cond(ArvoreGen *pai);
int fator_cond(ArvoreGen *pai);
int iter(ArvoreGen *pai);
int atrib(ArvoreGen *pai);
int exp(ArvoreGen *pai);
int exp_bool(ArvoreGen *pai);
int A(ArvoreGen *pai);
int termo_bool(ArvoreGen *pai);
int B(ArvoreGen *pai);
int fator_bool(ArvoreGen *pai);
int exp_rel(ArvoreGen *pai);
int C(ArvoreGen *pai);
int exp_arit(ArvoreGen *pai);
int D(ArvoreGen *pai);
int termo_arit(ArvoreGen *pai);
int E(ArvoreGen *pai);
int fator_arit(ArvoreGen *pai);
int list_func(ArvoreGen *pai);
int list_param(ArvoreGen *pai);
int param(ArvoreGen *pai);
int retorno(ArvoreGen *pai);
int ch_func(ArvoreGen *pai);
int confirmar_tk(Categoria c, ArvoreGen *pai);

#endif // SINTATICO_H_INCLUDED
