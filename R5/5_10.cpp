#include <iostream>
#include "ast.h"

int main()
{
    Expression *s = new Exp_variable("s");
    St_return *st_return = new St_return(s);
    st_return->print(std::cout);
}