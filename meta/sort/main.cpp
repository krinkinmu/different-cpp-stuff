#include <iostream>
#include <typeinfo>
#include <array>

template <int... vals>
struct ints { };



template <bool cond, int ifT, int ifF>
struct conditional
{
	static const int value = ifF;
};

template <int ifT, int ifF>
struct conditional<true, ifT, ifF>
{
	static const int value = ifT;
};



template <int first, int second>
struct get_min
{
	static const int value = conditional<(first < second), first, second>::value;
};



template <typename B, typename E>
struct concat;

template <int... begin, int... end>
struct concat<ints<begin...>, ints<end...>>
{
	typedef ints<begin..., end...> type;
};



template <typename T>
struct find_min;

template <int val>
struct find_min<ints<val>>
{ static const int value = val; };

template <int head, int... tail>
struct find_min<ints<head, tail...>>
{
	static const int value = get_min<head, find_min<ints<tail...>>::value>::value;
};



template <typename T, int val>
struct extract_value
{
	typedef ints<> value;
	typedef T other;
};

template <int... vals, int val>
struct extract_value<ints<val, vals...>, val>
{
	typedef ints<val> value;
	typedef ints<vals...> other;
};

template <int head, int... vals, int val>
struct extract_value<ints<head, vals...>, val>
{
	typedef typename extract_value<ints<vals...>, val>::value value;
	typedef typename concat<ints<head>, typename extract_value<ints<vals...>, val>::other>::type other;
};



template <typename T>
struct meta_sort;

template <>
struct meta_sort<ints<>>
{
	typedef ints<> type;
};

template <int val>
struct meta_sort<ints<val>>
{
	typedef ints<val> type;
};

template <int... vals>
struct meta_sort<ints<vals...>>
{
private:
	typedef ints<vals...> meta_array;
    typedef extract_value<meta_array, find_min<meta_array>::value> meta_extractor;
	typedef typename meta_extractor::value head;
	typedef typename meta_extractor::other tail;
	typedef typename meta_sort<tail>::type sorted_tail;
public:
	typedef typename concat<head, sorted_tail>::type type;
};



template <typename T>
struct from_ints;

template <int... vals>
struct from_ints<ints<vals...>>
{
	static const std::array<int, sizeof...(vals)> ar;
};

template <int... vals>
const std::array<int, sizeof...(vals)> from_ints<ints<vals...>>::ar = { { vals... } };


int main()
{
	typedef ints<10, 20, 30, -20, 40, 50, 60, 70, -10> array_t;

	for (int v : from_ints<meta_sort<array_t>::type>::ar)
		std::cout << v << std::endl;

	return 0;
}
