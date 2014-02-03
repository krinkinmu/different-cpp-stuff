#ifndef __MY_BIND_HPP__
#define __MY_BIND_HPP__

#include <functional>

template <typename T> struct unref { typedef T type; };
template <typename T> struct unref<T&> { typedef T type; };

template <typename T> struct unconst { typedef T type; };
template <typename T> struct unconst<T const> { typedef T type; };

template <typename T> struct uncv
{ typedef typename unconst<typename unref<T>::type>::type type; };

template <typename Op>
class mybinder1st
    : public std::unary_function<typename Op::second_argument_type,
                                 typename Op::result_type>
{
    private:

        typedef typename Op::first_argument_type first_type;
        typedef typename uncv<first_type>::type const & first_cref;

        typedef typename Op::second_argument_type second_type;
        typedef typename uncv<second_type>::type const & second_cref;
        typedef typename uncv<second_type>::type & second_ref;

        Op op;
        typename Op::first_argument_type value;

    public:
        mybinder1st(Op const & op, first_cref arg)
            : op(op), value(arg)
        {}

        typename Op::result_type operator()(second_cref arg) const
        { return op(value, arg); }

        typename Op::result_type operator()(second_ref arg) const
        { return op(value, arg); }
};

template <typename Op, typename T>
mybinder1st<Op> mybind1st(Op const & op, T const & arg)
{
    typedef typename uncv<typename Op::first_argument_type>::type const & arg_type;
    return mybinder1st<Op>(op, static_cast<arg_type>(arg));
}

#endif /*__MY_BIND_HPP__*/
