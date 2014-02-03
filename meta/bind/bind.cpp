#include <iostream>

namespace impl
{

	namespace detail
	{
		template <size_t I> struct arg { arg() { } };

		template <typename T> struct type { };

		template <typename T> struct value
		{
			value(T const & v) : data(v) { }
			T const & get() const { return data; }
			T data;
		};

		template <typename T> struct wrapper { typedef value<T> type; };

		template <size_t I> struct wrapper< arg<I> > { typedef arg<I> type; };

		struct list0
		{
			template <typename T> T const & operator[] (value<T> const & v) { return v.get(); }

			template <typename R, typename F, typename L> R operator()(type<R>, F f, L) const
			{ return f(); }

			template <typename F, typename L> void operator()(type<void>, F f, L) const
			{ f(); }
		};

		struct list0_helper { typedef list0 type; };

		template <typename A1> struct list1
		{
			list1(A1 a1) : a1_(a1) { }

			template <typename T> T const & operator[] (value<T> const & v) { return v.get(); }
			A1 const & operator[] (arg<1> const &) { return a1_; }

			template <typename R, typename F, typename L> R operator()(type<R>, F f, L l) const
			{ return f(l[a1_]); }

			template <typename F, typename L> void operator()(type<void>, F f, L l) const
			{ f(l[a1_]); }

			A1 a1_;
		};

		template <typename A1>
		struct list1_helper
		{
			typedef typename wrapper<A1>::type A1_;
			typedef list1<A1_> type;
		};

		template <typename A1, typename A2> struct list2
		{
			list2(A1 a1, A2 a2) : a1_(a1), a2_(a2) { }

			template <typename T> T const & operator[] (value<T> const & v) { return v.get(); }
			A1 const & operator[] (arg<1> const &) { return a1_; }
			A2 const & operator[] (arg<2> const &) { return a2_; }

			template <typename R, typename F, typename L> R operator()(type<R>, F f, L l) const
			{ return f(l[a1_], l[a2_]); }

			template <typename F, typename L> void operator()(type<void>, F f, L l) const
			{ f(l[a1_], l[a2_]); }

			A1 a1_;
			A2 a2_;
		};

		template <typename A1, typename A2>
		struct list2_helper
		{
			typedef typename wrapper<A1>::type A1_;
			typedef typename wrapper<A2>::type A2_;
			typedef list2<A1_, A2_> type;
		};

		template <typename R, typename F, typename L>
		struct bind_t
		{
			bind_t(F f, L l) : f_(f), l_(l) { }

			R operator()() const
			{
				list0 args;
				return l_(type<R>(), f_, args);
			}

			template <typename A1>
			R operator()(A1 a1) const
			{
				list1<A1> args(a1);
				return l_(type<R>(), f_, args);
			}

			template <typename A1, typename A2>
			R operator()(A1 a1, A2 a2) const
			{
				list2<A1, A2> args(a1, a2);
				return l_(type<R>(), f_, args);
			}

			F f_;
			L l_;
		};

	}

	static const detail::arg<1> _1;
	static const detail::arg<2> _2;

	template <typename R>
	detail::bind_t<R, R(*)(void), detail::list0_helper::type> bind(R(*f)(void))
	{ return detail::bind_t<R, R(*)(void), detail::list0_helper::type>(f, detail::list0_helper::type()); }

	template <typename R, typename T1, typename A1>
	detail::bind_t<R, R(*)(T1), typename detail::list1_helper<A1>::type> bind(R(*f)(T1), A1 a1)
	{ return detail::bind_t<R, R(*)(T1), typename detail::list1_helper<A1>::type>(f, typename detail::list1_helper<A1>::type(a1)); }

	template <typename R, typename T1, typename A1, typename T2, typename A2>
	detail::bind_t<R, R(*)(T1, T2), typename detail::list2_helper<A1, A2>::type> bind(R(*f)(T1, T2), A1 a1, A2 a2)
	{ return detail::bind_t<R, R(*)(T1, T2), typename detail::list2_helper<A1, A2>::type>(f, typename detail::list2_helper<A1, A2>::type(a1, a2)); }

}

int foo() { std::cout << "int foo();" << std::endl; return 0; }
int foo_i(int i1) { std::cout << "int foo(" << i1 << ");" << std::endl; return i1; }
int foo_ii(int i1, int i2) { std::cout << "int foo(" << i1 << ", " << i2 << ");" << std::endl; return i1 + i2; }

int main()
{
	std::cout << impl::bind(&foo)() << std::endl;
	std::cout << impl::bind(&foo_i, 10)() << std::endl;
	std::cout << impl::bind(&foo_i, impl::_1)(10) << std::endl;
	std::cout << impl::bind(&foo_ii, 10, 5)() << std::endl;
	std::cout << impl::bind(&foo_ii, 10, impl::_1)(5) << std::endl;
	std::cout << impl::bind(&foo_ii, impl::_1, 5)(10) << std::endl;
	std::cout << impl::bind(&foo_ii, impl::_1, impl::_2)(10, 5) << std::endl;
	std::cout << impl::bind(&foo_ii, impl::_2, impl::_1)(10, 5) << std::endl;

	return 0;
}
