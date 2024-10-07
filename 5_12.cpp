#include <iostream>
#include "ast.h"

int main()
{
    Variable *d = new Variable(Type_INT, "a");
    d->print(std::cout);
}