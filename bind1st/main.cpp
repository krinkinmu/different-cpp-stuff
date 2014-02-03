#include "mybind.hpp"

#include <functional>
#include <iostream>
#include <string>

int compare(std::string const & l, std::string const & r)
{
    if (l < r)
        return -1;
    if (l == r)
        return 0;
    return 1;
}

int main()
{
    std::string const hello("hello");
    //std::cout << std::bind1st(std::ptr_fun(&compare), hello)("World!") << std::endl;  // error here
    std::cout << mybind1st(std::ptr_fun(&compare), hello)("World!") << std::endl;

    return 0;
}
