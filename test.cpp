#include <iostream>
#include <string>

int main(void)
{
    std::string a = "C";
    std::string b = " ";
    std::string c = "programming";

    std::string x = a + b + c;
    std::string y = a + "++" + b + c;
    std::string z = a;
    z += b;
    z += c;

    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "z = " << z << std::endl;

    if (x == y)
        std::cout << "x==y\n";
    else if (x < y)
        std::cout << "x<y\n";
    else
        std::cout << "x>y\n";

    if (x == z)
        std::cout << "x==z\n";
    else if (x < z)
        std::cout << "x<z\n";
    else
        std::cout << "x>z\n";
    return 0;
}