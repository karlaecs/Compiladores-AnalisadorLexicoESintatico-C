#include<stdio.h>
#include<stdlib.h>
#include "sintatico.h"
#include "../Analisador_Léxico/AnalisadorLexico.h"

Token *token;

/*
A chamada para a principal função, a partir dela uma variavel global token que é auxiliar, será alocada e ficará
recebendo os tokens analisados pelo Lexico,eles serão atualizados conforme o sintatico for solicitando.
*/
void analisadorSintatico() {
    token = (Token*) malloc(sizeof(Token));
    token = proximo_token();
    programa();

}

int corpo() {
    if(token != NULL){
        if(token->categoria == tk_open_key) {
            token = proximo_token();
            if(cmd()) {
                if(token->categoria == tk_close_key) {
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
	if(termo_program()) {
			if(token->categoria == tk_kw_main) {
			    token = proximo_token();
				if(corpo()) {
                    puts("Executado com Sucesso!");
					return 1;
                }
			}
			return 0;
	}
	return 0;
}

int termo_program() {

	if(list_func())
		return 1;
	return 2;
}

int cmd() {

	if(var_dec()) {
        if(cmd())
            return 1;
        return 0;
    }

	else if(cond()) {
        if(cmd())
            return 1;
        return 0;
    }
	else if(iter()) {
        if(cmd())
            return 1;
        return 0;
    }
	else if(atrib()){
        if(token->categoria == tk_semicolon) {
            token = proximo_token();
            if(cmd())
                return 1;
            return 0;
        }
        puts("ERRO - FALTA ;");
        exit(EXIT_FAILURE);
    }
	else if(ch_func()) {
        if(cmd())
            return 1;
        return 0;
    }
    return 2;
}

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
}

int var_dec()
{
    if(type()) {
        if(token->categoria == tk_id) {
            token = proximo_token();
            if(termo_var_dec()) {
                if(token->categoria == tk_semicolon) {
                    token = proximo_token();
                    return 1;
                }
                 puts("ERRO FALTA ;");
                 exit(EXIT_FAILURE);
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
        if(token->categoria == tk_open_bra){
            token = proximo_token();
            if(token->categoria == tk_const_int){
                token = proximo_token();
                if(token->categoria == tk_close_bra)
                    return 1;
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }
        return 2;
    }
    return 0;

}

int type() {

    if(token != NULL){
        if(token->categoria == tk_kw_int || token->categoria == tk_kw_float || token->categoria == tk_kw_char) {
            token = proximo_token();
            return 1;
        }
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
        if(A())
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
        if(token->categoria ==  tk_kw_and) {
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
        if(token->categoria == tk_const_int || token->categoria == tk_const_float ||  token->categoria == tk_id){
            token = proximo_token();
            return 1;
        }
    }
    return 0;
}

int list_func() {
        if(token != NULL){
            if(token->categoria == tk_kw_func) {
                token = proximo_token();
                if(token->categoria == tk_id) {
                    token = proximo_token();
                    if(token->categoria == tk_open_par) {
                        token = proximo_token();
                        if(list_param()) {
                            if(token->categoria == tk_close_par) {
                                token = proximo_token();
                                if(token->categoria == tk_open_key) {
                                    token = proximo_token();
                                    if(cmd()){
                                        if(token->categoria == tk_kw_return) {
                                            token = proximo_token();
                                            if(exp()) {
                                                if(token->categoria == tk_semicolon) {
                                                    token = proximo_token();
                                                    if(token->categoria == tk_close_key) {
                                                        token = proximo_token();
                                                        if(list_func()){
                                                            return 1;
                                                        }
                                                        return 0;
                                                    }
                                                    puts("ERRO - Falta }");
                                                    exit(EXIT_FAILURE);
                                                }
                                                puts("ERRO - Falta ;3");
                                                exit(EXIT_FAILURE);
                                            }
                                            return 0;
                                        }
                                        return 0;
                                    }
                                    return 0;
                                }
                                puts("ERRO - Falta {");
                                exit(EXIT_FAILURE);
                            }
                            puts("ERRO - Falta )");
                            exit(EXIT_FAILURE);
                        }
                        return 0;
                    }
                    puts("ERRO - Falta (");
                    exit(EXIT_FAILURE);
                }
                return 0;
            }
            return 2;
        }
        return 2;

}

int list_param() {
    if(type()) {
        if(token->categoria == tk_id) {
            token = proximo_token();
            if(param())
                return 1;
            return 0;
        }
        return 0;
    }
    return 0;
}

int param() {
    if(token != NULL){
        if(token->categoria == tk_comma) {
            token = proximo_token();
            if(list_param())
                return 1;
            return 0;
        }
        else if(token->categoria != tk_close_par){
            puts("ERRO - Falta ,");
            return 0;
        }
    }
    return 2;
}
