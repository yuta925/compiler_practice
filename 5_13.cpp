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

Function *make_function_asum()
{
    // args の生成
    std::list<Variable *> args;
    args.push_back(new Variable(Type_INT, "n"));

    // lvarlist の生成
    std::list<Variable *> lvarlist;
    lvarlist.push_back(new Variable(Type_INT, "s"));
    lvarlist.push_back(new Variable(Type_INT, "i"));

    // body の生成
    Exp_variable *s = new Exp_variable("s");
    Exp_variable *i = new Exp_variable("i");
    Exp_variable *n = new Exp_variable("n");
    Exp_constant *zero = new Exp_constant(Type_INT, 0);
    Exp_operation1 *minus_n = new Exp_operation1(Operator_MINUS, n);

    St_assign *init_s = new St_assign(s, zero);
    St_assign *init_i = new St_assign(i, minus_n);
    Statement *st_while = make_while();
    St_return *ret = new St_return(s);

    std::list<Statement *> st_list;
    st_list.push_back(init_s);
    st_list.push_back(init_i);
    st_list.push_back(st_while);
    st_list.push_back(ret);
    Statement *body = new St_list(st_list);

    return new Function(Type_INT, "asum", args, lvarlist, body);
}

int main()
{
    Function *func_asume = make_function_asum();
    func_asume->print(std::cout);
    return 0;
}