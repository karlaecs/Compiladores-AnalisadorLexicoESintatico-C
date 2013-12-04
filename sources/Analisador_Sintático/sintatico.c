#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sintatico.h"


#define INTMAX 35


Token *token;
FILE *arq;
ArvoreGen *inicial;
char str[]= "id";
/*
A chamada para a principal função, a partir dela uma variavel global token que é auxiliar, será alocada e ficará
recebendo os tokens analisados pelo Lexico,eles serão atualizados conforme o sintatico for solicitando.
*/
void analisador_sintatico() {

    arq = fopen("../tree_tex/arvore.tex", "w");
    if(arq == NULL) {
        puts("Falha ao abrir arvore.tex");
    }
    token = (Token*) malloc(sizeof(Token));
    token = proximo_token();
    programa();
    imprime(inicial);
    fclose(arq);
}

int confirmar_tk(Categoria c, ArvoreGen *pai) {
	if(c == token->categoria) {
		ArvoreGen *no;
		if(c== tk_id)
			no = criar(strcat(str, token->valor));
		else
			no = criar(token->valor);
		inserir(no, pai);
		token = proximo_token();
		return 1;
	}
	return 0;
}

int corpo(ArvoreGen *pai) {
	ArvoreGen *no = criar("corpo");
	inserir(no, pai);
	if(confirmar_tk(tk_open_key, no) && cmd(no) && confirmar_tk(tk_close_key, no))
		return 1;

	return 0;
}

int programa() {
	inicial = criar("Programa");
	if(termo_program(inicial) && confirmar_tk(tk_kw_main, inicial) && corpo(inicial))
		return 1;
	return 0;
}

int termo_program(ArvoreGen *pai) {
	ArvoreGen *no = criar("termo\\_programa");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_func, no) && confirmar_tk(tk_id, no) && confirmar_tk(tk_open_par, no) && list_param(no) && confirmar_tk(tk_open_key, no) && cmd(no) && confirmar_tk(tk_kw_return, no) && retorno(no) && exp_bool(no) && confirmar_tk(tk_semicolon, no) && confirmar_tk(tk_close_key, no) && termo_program(no))
	return 1;

	inserir(criar("vazio"), no);
	return 1;
}

int retorno(ArvoreGen *pai) {
	ArvoreGen *no = criar("retorno");
	inserir(no, pai);
	if(exp_bool(no) || confirmar_tk(tk_const_lit, no))
		return 1;
	pai->prim = NULL;
    free(no);
	return 0;
}

int list_param(ArvoreGen *pai) {
	ArvoreGen *no = criar("list_param");
	inserir(no, pai);
	if(type(no) && confirmar_tk(tk_id, no) && param(no))
		return 1;
	pai->prim = NULL;
    free(no);
	return 0;
}

int param(ArvoreGen *pai) {
	ArvoreGen *no = criar("param");
	inserir(no, pai);
	if(confirmar_tk(tk_comma, no) && list_param(no))
		return 1;

	inserir(criar("vazio"), no);
	return 1;
}

int type(ArvoreGen *pai) {
	ArvoreGen *no = criar("type");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_int, no) || confirmar_tk(tk_kw_float, no) || confirmar_tk(tk_kw_char, no))
		return 1;
	pai->prim = NULL;
    free(no);
	return 0;
}

int cmd(ArvoreGen *pai) {
	//puts("cmd");
	ArvoreGen *no = criar("cmd");
	inserir(no, pai);
	if((var_dec(no) && cmd(no)) || (cond(no) && cmd(no)) || (iter(no) && cmd(no)) || (atrib(no) && cmd(no)) || (ch_func(no) && cmd(no)) )
		return 1;

	inserir(criar("vazio"), no);
	return 1;
}

int ch_func(ArvoreGen *pai) {
//puts("ch func");
	return 0;
}

int var_dec(ArvoreGen *pai) {
		//puts("var_dec");
	ArvoreGen *no = criar("var\\_dec");
	inserir(no, pai);
	if(type(no) && confirmar_tk(tk_id, no) && termo_var_dec(no) && confirmar_tk(tk_semicolon, no))
		return 1;

	pai->prim = NULL;
    free(no);
	return 0;
}

int termo_var_dec(ArvoreGen *pai) {
	ArvoreGen *no = criar("termo\\_var\\_dec");
	inserir(no, pai);
	if(confirmar_tk(tk_open_bra, no) && confirmar_tk(tk_const_int, no) && confirmar_tk(tk_close_bra, no))
		return 1;

	inserir(criar("vazio"), no);
	return 1;
}

int cond(ArvoreGen *pai) {
	//puts("cond");
	ArvoreGen *no = criar("cond");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_if, no) && confirmar_tk(tk_open_par, no) && exp_bool(no) && confirmar_tk(tk_close_par, no) && corpo(no) && termo_cond(no))
		return 1;
	pai->prim = NULL;
    free(no);
	return 0;
}

int termo_cond(ArvoreGen *pai) {
	ArvoreGen *no = criar("termo\\_cond");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_else, no) && fator_cond(no))
		return 1;

	inserir(criar("vazio"), no);
	return 1;
}

int fator_cond(ArvoreGen *pai) {
	ArvoreGen *no = criar("fator\\_cond");
	inserir(no, pai);
	if(corpo(no) || cond(no))
		return 1;
	pai->prim = NULL;
    free(no);
	return 0;
}

int iter (ArvoreGen *pai) {
	//puts("iter");
    ArvoreGen *no = criar("iter");
    inserir(no, pai);
    if(confirmar_tk(tk_kw_for, no) && confirmar_tk(tk_open_par, no) && atrib(no) && confirmar_tk(tk_semicolon, no) && exp_bool(no) && confirmar_tk(tk_semicolon, no) && atrib(no) && confirmar_tk(tk_close_par, no) && corpo(no))
        return 1;
    else if(confirmar_tk(tk_kw_while, no) && confirmar_tk(tk_open_par, no) && exp_bool(no) && confirmar_tk(tk_close_par, no) && corpo(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int atrib(ArvoreGen *pai) {
    ArvoreGen *no = criar("atrib");
	inserir(no, pai);
    if(confirmar_tk(tk_id, no) && confirmar_tk(tk_op_atrib, no) && exp_arit(no)) {
        return 1;
    }
    pai->prim = NULL;
    free(no);
    return 0;
}

int exp_bool(ArvoreGen *pai) {
    ArvoreGen *no = criar("exp\\_bool");
	inserir(no, pai);
	if(termo_bool(no) && A(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int A(ArvoreGen *pai) {
    ArvoreGen *no = criar("A");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_or, no) && termo_bool(no) && A(no))
        return 1;

    inserir(criar("vazio"), no);
    return 1;
}

int termo_bool(ArvoreGen *pai) {
    ArvoreGen *no = criar("termo\\_bool");
	inserir(no, pai);
    if(fator_bool(no) && B(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int B(ArvoreGen *pai) {
    ArvoreGen *no = criar("B");
	inserir(no, pai);
	if(confirmar_tk(tk_kw_and, no) && fator_bool(no) && B(no))
        return 1;

    inserir(criar("vazio"), no);
    return 1;
}

int fator_bool(ArvoreGen *pai) {
    ArvoreGen *no = criar("fator\\_bool");
	inserir(no, pai);
    if((confirmar_tk(tk_kw_not, no) && fator_bool(no)) || exp_rel(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int exp_rel(ArvoreGen *pai) {
    ArvoreGen *no = criar("exp\\_rel");
	inserir(no, pai);
	if(exp_arit(no) && C(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int C(ArvoreGen *pai) {
    ArvoreGen *no = criar("C");
	inserir(no, pai);
	if(confirmar_tk(tk_rel_op, no) && exp_arit(no))
        return 1;
    inserir(criar("vazio"), no);
    return 1;
}

int exp_arit(ArvoreGen *pai) {
    ArvoreGen *no = criar("exp\\_arit");
	inserir(no, pai);
	if(termo_arit(no) && D(no))
        return 1;

	pai->prim = NULL;
    free(no);
    return 0;
}

int D(ArvoreGen *pai) {
    ArvoreGen *no = criar("D");
	inserir(no, pai);
	if(confirmar_tk(tk_op_add, no) || confirmar_tk(tk_op_sub, no)){
        if(termo_arit(no) && D(no))
            return 1;
        return 0;
	}
    inserir(criar("vazio"), no);
    return 1;
}

int termo_arit(ArvoreGen *pai) {
    ArvoreGen *no = criar("termo\\_arit");
	inserir(no, pai);
	if(fator_arit(no) && E(no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}

int E(ArvoreGen *pai) {
    ArvoreGen *no = criar("E");
	inserir(no, pai);
	if(confirmar_tk(tk_op_mul, no) || confirmar_tk(tk_op_div, no)){
        if(fator_arit(no) && E(no))
            return 1;
        return 0;
	}
    inserir(criar("vazio"), no);
    return 1;
}

int fator_arit(ArvoreGen *pai) {
    ArvoreGen *no = criar("fator\\_arit");
	inserir(no, pai);
	if(confirmar_tk(tk_const_int, no) || confirmar_tk(tk_const_float, no) || confirmar_tk(tk_id, no))
        return 1;
	pai->prim = NULL;
    free(no);
    return 0;
}


