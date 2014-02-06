#include <iostream>
#include <cstddef>

template <typename T, size_t N>
size_t size(T (&)[N])
{ return N; }

int main()
{
	int ar1[] = { 0, 1, 2, 3 };
	extern int ar2[];
	int * ar3 = ar1;

	std::cout << size(ar1) << std::endl;
	//std::cout << size(ar2) << std::endl; //ERROR
	//std::cout << size(ar3) << std::endl; //ERROR

	(void) ar3; // compiler warning

	return 0;
}
