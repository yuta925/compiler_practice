#include <iostream>
#include "ast.h"

int main()
{
    std::list<Variable *> gvar;
    std::list<Variable *> func;
    gvar.push_back(new Variable(Type_INT, "g"));
    func.push_back(make_function_asum());
    Program *p = new Program(gvar, func, make_function_main());
    p->run();
}