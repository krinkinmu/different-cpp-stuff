#include <iostream>
#include <type_traits>
#include <string>

namespace detail
{
	template <typename T>
	T min_impl(T&& arg)
	{
		return std::forward<T>(arg);
	}

	template <typename T1, typename T2>
	typename std::common_type<T1, T2>::type min_impl(T1&& t1, T2&& t2)
	{
		return (t1 < t2) ? t1 : t2;
	}

	template <typename T, typename ... Other>
	typename std::common_type<T, Other...>::type min_impl(T&& t, Other&& ... other)
	{
		return min_impl(std::forward<T>(t), min_impl(std::forward<Other>(other)...));
	}
}

template <typename ... Args>
typename std::common_type<Args...>::type min(Args&& ... args)
{
	return detail::min_impl(std::forward<Args>(args)...);
}

int main()
{
	char const *ptr1 = "10";
	char const *ptr2 = "20";
	std::cout << min(ptr2, ptr1, std::string("30")) << std::endl;
	return 0;
}
