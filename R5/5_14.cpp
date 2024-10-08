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
    Exp_variable *i = new Exp_variable("i");
    Exp_variable *n = new Exp_variable("n");
    Expression *cond = new Exp_operation2(Operator_LE, i, n);

    Statement *body = make_if();
    return new St_while(cond, body);
}

Function *make_function_asum()
{
    // args の生成
    std::list<Variable *> args;
    args.push_back(new Variable(Type_INT, "n"));

    // lvarlist の生成
    std::list<Variable *> lvarlist;
    lvarlist.push_back(new Variable(Type_INT, "i"));
    lvarlist.push_back(new Variable(Type_INT, "s"));

    // body の生成
    Exp_variable *s = new Exp_variable("s");
    Exp_variable *i = new Exp_variable("i");
    Exp_variable *n = new Exp_variable("n");
    Exp_constant *zero = new Exp_constant(Type_INT, 0);
    Exp_operation1 *minus_n = new Exp_operation1(Operator_MINUS, n);

    St_assign *init_i = new St_assign(s, zero);
    St_assign *init_s = new St_assign(i, minus_n);
    Statement *st_while = make_while();
    St_return *ret = new St_return(s);

    std::list<Statement *> st_list;
    st_list.push_back(init_i);
    st_list.push_back(init_s);
    st_list.push_back(st_while);
    st_list.push_back(ret);
    Statement *body = new St_list(st_list);

    return new Function(Type_INT, "asum", args, lvarlist, body);
}

int main()
{
    // グローバル変数の生成
    Variable *global_g = new Variable(Type_INT, "g");
    std::list<Variable *> var_list;
    var_list.push_back(global_g);

    // asum関数の生成
    Function *asum = make_function_asum();

    // main関数の作成
    std::list<Variable *> main_args;

    std::list<Variable *> lvarlist;
    lvarlist.push_back(new Variable(Type_INT, "a"));

    Exp_variable *a = new Exp_variable("a");

    Exp_variable *g = new Exp_variable("g");
    St_assign *init_g = new St_assign(g, new Exp_constant(Type_INT, 3));

    std::list<Expression *> asum_args;
    asum_args.push_back(g);
    St_assign *init_a = new St_assign(a, new Exp_function("asum", asum_args));

    std::list<Expression *> putint_args;
    putint_args.push_back(a);
    Statement *putint = new St_function("putint", putint_args);

    std::list<Statement *> body;
    body.push_back(init_g);
    body.push_back(init_a);
    body.push_back(putint);
    St_list *st_list = new St_list(body);

    Function *main = new Function(Type_INT, "main", main_args, lvarlist, st_list);

    std::list<Function *> func_list;
    func_list.push_back(asum);

    Program *prog = new Program(var_list, func_list, main);
    prog->print(std::cout);
}