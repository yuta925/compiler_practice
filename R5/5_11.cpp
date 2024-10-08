#include <iostream>
#include "ast.h"

int main()
{
    // putint(a);と出力する
    Exp_variable *a = new Exp_variable("a");
    std::list<Expression *> args;
    args.push_back(a);
    St_function *putint = new St_function("putint", args);
    putint->print(std::cout);
}