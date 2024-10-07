#include <iostream>
#include "ast.h"

Statement *make_if()
{
    Exp_variable *i = new Exp_variable("i");
    Exp_variable *s = new Exp_variable("s");
    Exp_constant *zero = new Exp_constant(Type_INT, 0);

    // cond 部の木の生成
    Exp_operation2 *cond = new Exp_operation2(Operator_LT, i, zero);

    // then 部の木の生成
    Exp_operation2 *then_exp = new Exp_operation2(Operator_MINUS, s, i);
    Statement *then = new St_assign(s, then_exp);

    // else 部の木の生成
    Exp_operation2 *else_exp = new Exp_operation2(Operator_PLUS, s, i);
    Statement *els = new St_assign(s, else_exp);

    return new St_if(cond, then, els);
}

int main()
{
    Statement *s = make_if();
    s->print(std::cout);
    return 0;
}