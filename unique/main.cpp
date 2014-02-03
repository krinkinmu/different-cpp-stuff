#include <iostream>
#include <cstdlib>

#include "unique_ptr.hpp"

unique_ptr<int> new_int() { return unique_ptr<int>(new int(10)); }
unique_ptr<int, array_deleter<int> > new_int_array() { return unique_ptr<int, array_deleter<int> > (new int[10]); }
unique_ptr<int, void(*)(void *)> malloc_int() { return unique_ptr<int, void(*)(void *)>((int *)malloc(sizeof(int)), &free); }

int main()
{
    unique_ptr<int> v1 = new_int();
    unique_ptr<int, array_deleter<int> > v2 = new_int_array();
    unique_ptr<int, void(*)(void *)> v3 = malloc_int();

    return 0;
}
