#ifndef SINTATICO_H_INCLUDED
#define SINTATICO_H_INCLUDED

void analisadorSintatico();
int corpo();
int programa();
int termo_program();
int cmd();
int var_dec();
int termo_var_dec();
int type();
int cond();
int termo_cond();
int fator_cond();
int iter();
int atrib();
int exp();
int exp_bool();
int A();
int termo_bool();
int B();
int fator_bool();
int exp_rel();
int C();
int exp_arit();
int D();
int termo_arit();
int E();
int fator_arit();
int list_func();
int list_param();
int param();



#endif // SINTATICO_H_INCLUDED
