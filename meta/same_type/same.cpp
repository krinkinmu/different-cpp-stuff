#include <iostream>

template <typename LT, typename RT>
struct same_type
{ static bool const value = false; };

template <typename T>
struct same_type<T, T>
{ static bool const value = true; };

template <typename T>
struct unref
{ typedef T type; };

template <typename T>
struct unref<T&>
{ typedef T type; };

int main()
{
    std::cout << "int == float       = " << same_type<int, float>::value << std::endl
              << "int == int&        = " << same_type<int, int&>::value << std::endl
              << "int == int const   = " << same_type<int, int const>::value << std::endl
              << "int == int const & = " << same_type<int, int const &>::value << std::endl
              << "int == int         = " << same_type<int, int>::value << std::endl;

    std::cout << "unref<int>                 == int               = " << same_type<unref<int>::type, int>::value << std::endl
              << "unref<int &>               == int               = " << same_type<unref<int &>::type, int>::value << std::endl
              << "unref<int const &>         == int               = " << same_type<unref<int const &>::type, int>::value << std::endl
              << "unref<int const &>         == int const         = " << same_type<unref<int const &>::type, int const>::value << std::endl
              << "unref<int * &>             == int *             = " << same_type<unref<int * &>::type, int *>::value << std::endl
              << "unref<int const * &>       == int const *       = " << same_type<unref<int const * &>::type, int const *>::value << std::endl
              << "unref<int const * const &> == int const * const = " << same_type<unref<int const * const &>::type, int const * const>::value << std::endl
              << "unref<int const * const &> == int const *       = " << same_type<unref<int const * const &>::type, int const *>::value << std::endl;

    return 0;
}
