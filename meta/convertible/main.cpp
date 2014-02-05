#include <iostream>
#include <iomanip>

template <typename From, typename To>
class is_implicitly_convertible
{
	typedef char false_type_tmp;
	struct true_type_tmp { false_type_tmp dummy[2]; };

	static true_type_tmp check(To)
	{ return true_type_tmp(); }

	static false_type_tmp check(...)
	{ return false_type_tmp(); }

public:
	static const bool value = (sizeof(check(*reinterpret_cast<From*>(0))) == sizeof(true_type_tmp));
};

struct Base { };
struct Derived : Base { };

int main()
{
	std::cout << std::boolalpha;

	std::cout << "is_implucitly_convertible<int, long>::value = " << is_implicitly_convertible<int, long>::value << std::endl;
	std::cout << "is_implicitly_convertible<int, int>::value = " << is_implicitly_convertible<int, int>::value << std::endl;
	std::cout << "is_implicitly_convertible<Base, Derived>::value = " << is_implicitly_convertible<Base, Derived>::value << std::endl;

	std::cout << "is_implicitly_convertible<int, Base>::value = " << is_implicitly_convertible<int, Base>::value << std::endl;
	std::cout << "is_implicitly_convertible<Derived, Base>::value = " << is_implicitly_convertible<Derived, Base>::value << std::endl;
	std::cout << "is_implicitly_convertible<long, int>::value = " << is_implicitly_convertible<long, int>::value << std::endl;

	return 0;
}
