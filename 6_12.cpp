#include <iostream>
#include "ast.h"

Statement *make_if()
{
    // cond: i<0
    Expression *cond = new Exp_operation2(
        Operator_LT,
        new Exp_variable("i"),
        new Exp_constant(Type_INT, 0));

    // then: s = s - i;
    Statement *then = new St_assign(
        new Exp_variable("s"),
        new Exp_operation2(
            Operator_MINUS,
            new Exp_variable("s"),
            new Exp_variable("i")));

    // else s = s + i;
    Statement *els = new St_assign(
        new Exp_variable("s"),
        new Exp_operation2(
            Operator_PLUS,
            new Exp_variable("s"),
            new Exp_variable("i")));

    Statement *els2 = new St_return(new Exp_variable("s"));

    return new St_if(cond, then, els2);
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

    // 変数の初期化
    St_assign *init_s = new St_assign(s, zero);    // s を 0 に初期化
    St_assign *init_i = new St_assign(i, minus_n); // i を -n に初期化
    Statement *st_while = make_while();
    St_return *ret = new St_return(s);

    std::list<Statement *> st_list;
    st_list.push_back(init_s); // s の初期化を先に
    st_list.push_back(init_i); // i の初期化を後に
    st_list.push_back(st_while);
    st_list.push_back(ret);
    Statement *body = new St_list(st_list);

    return new Function(Type_INT, "asum", args, lvarlist, body);
}

int main(void)
{
    Function *function_asum = make_function_asum();

    std::map<std::string, Function *> func;
    std::map<std::string, int> gvar;
    std::list<int> i_arglist;
    i_arglist.push_back(3);
    int r = function_asum->run(func, gvar, i_arglist);
    std::cout << r << std::endl;
}

// std::list<Expression *> arglist;
// arglist.push_back(new Exp_constant(Type_INT, 5));
// Expression *exp_function_asum = new Exp_function("asum", arglist);

// std::map<std::string, Function *> func;
// std::map<std::string, int> gvar;
// std::map<std::string, int> lavar;
// func["asum"] = make_function_asum();
// int asum5 = exp_function_asum->run(func, gvar, lavar);
// std::cout << "asum(5) = " << asum5 << std::endl;