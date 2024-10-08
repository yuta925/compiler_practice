#include <iostream>
#include "ast.h"

int main()
{

    Expression *v = new Exp_variable("n");
    Expression *o = new Exp_operation1(Operator_MINUS, v);

    std::map<std::string, Function *> func;
    std::map<std::string, int> gvar;
    std::map<std::string, int> lvar;
    lvar["n"] = 31;
    std::cout << o->run(func, gvar, lvar) << std::endl;
}