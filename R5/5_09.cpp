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

Statement *make_while()
{
    // cond: i<=n
    Expression *v11 = new Exp_variable("i");
    Expression *v12 = new Exp_variable("n");
    Expression *cond = new Exp_operation2(Operator_LE, v11, v12);

    // body
    std::list<Statement *> slist;
    // if 文
    Statement *b1 = make_if();
    slist.push_back(b1);
    // i = i + 1;
    Expression *v21 = new Exp_variable("i");
    Expression *c2 = new Exp_constant(Type_INT, 1);
    Expression *e2 = new Exp_operation2(Operator_PLUS, v21, c2);
    Exp_variable *v22 = new Exp_variable("i");
    Statement *b2 = new St_assign(v22, e2);
    slist.push_back(b2);
    Statement *body = new St_list(slist);

    // while 文全体
    Statement *s = new St_while(cond, body);
    return s;
}

int main()
{
    Statement *st_while = make_while();
    st_while->print(std::cout, 0);
    return 0;
}