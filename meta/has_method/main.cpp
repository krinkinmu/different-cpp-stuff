#include <vector>
#include <iostream>
#include <iomanip>

template <typename T, T v>
struct integral_constant
{
	typedef integral_constant<T, v> type;
	const static T value = v;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

#define GEN_HAS_METHOD_CHECKER(MethodName, CheckerName)				\
namespace detail {													\
	template <typename T>											\
	class CheckerName##Impl {										\
		typedef char temp_true_type;								\
		struct temp_false_type { temp_true_type dummy[2]; };		\
		template <typename U, U> struct Check;						\
																	\
		struct Fallback { void MethodName() { } };					\
		struct Derived : T, Fallback { };							\
																	\
		template <typename U> static temp_false_type				\
			check(Check<void (Fallback::*)(), &U::MethodName> *)	\
				{ return temp_false_type(); }						\
		template <typename U> static temp_true_type					\
			check(...)												\
				{ return temp_true_type(); }						\
																	\
	public:															\
		static const bool value = (sizeof(check<Derived>(0))		\
									== sizeof(temp_true_type));		\
	};																\
}																	\
template <typename T>												\
struct CheckerName													\
	: integral_constant<bool, detail::CheckerName##Impl<T>::value>	\
{}

#define HAS_METHOD_CHECKER(MethodName)								\
	GEN_HAS_METHOD_CHECKER(MethodName, has_##MethodName)

HAS_METHOD_CHECKER(foo);

struct with_simple_foo { void foo() { std::cout << "i'm foo" << std::endl; } };
struct with_foo_int { void foo(int); };
struct with_multiple_foo { void foo(int); void foo(double); };
struct without_foo { void bar(); };

template <typename T>
void call_foo_helper(T t, true_type)
{
	t.foo();
}

template <typename T>
void call_foo_helper(T, false_type)
{
	std::cout << "there is no foo!" << std::endl;
}

template <typename T>
void call_foo(T const & t)
{
	call_foo_helper(t, typename has_foo<T>::type());
}

int main()
{
	std::cout << std::boolalpha;

	std::cout << "struct with_simple_foo has foo method = "
				<< has_foo<with_simple_foo>::value << std::endl;
	std::cout << "struct with_foo_int has foo method = "
				<< has_foo<with_foo_int>::value << std::endl;
	std::cout << "struct with_multiple_foo has foo method = "
				<< has_foo<with_multiple_foo>::value << std::endl;
	std::cout << "struct without_foo has foo method = "
				<< has_foo<without_foo>::value << std::endl;

	call_foo(with_simple_foo());
	call_foo(without_foo());

	return 0;
}
