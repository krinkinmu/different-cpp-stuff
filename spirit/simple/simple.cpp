#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <iostream>
#include <string>

namespace ast
{
    struct zero { };
    struct boolean { bool value; };

    struct succ_expr;
    struct pred_expr;
    struct iszero_expr;
    struct if_expr;

    typedef boost::variant<
        zero,
        boolean,
        boost::recursive_wrapper<succ_expr>,
        boost::recursive_wrapper<pred_expr>,
        boost::recursive_wrapper<iszero_expr>,
        boost::recursive_wrapper<if_expr>
        > term;

    struct succ_expr { term value; };
    struct pred_expr { term value; };
    struct iszero_expr { term value; };

    struct if_expr {
        term condition;
        term if_true;
        term if_false;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    ast::boolean,
    (bool, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    ast::succ_expr,
    (ast::term, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    ast::pred_expr,
    (ast::term, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    ast::iszero_expr,
    (ast::term, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    ast::if_expr,
    (ast::term, condition)
        (ast::term, if_true)
        (ast::term, if_false)
    )

namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    struct simple : qi::grammar<Iterator, ast::term(), ascii::space_type>
    {
        simple() : simple::base_type(term_parser)
        {
            using qi::char_;
            using qi::bool_;
            using qi::_val;
            using qi::omit;
            using qi::lit;

            zero_parser = omit[char_('0')] [ _val = ast::zero() ];
            boolean_parser = bool_;
            succ_parser = omit[lit("succ")] >> term_parser;
            pred_parser = omit[lit("pred")] >> term_parser;
            iszero_parser = omit[lit("iszero")] >> term_parser;
            if_parser = omit[lit("if")] >> term_parser >> omit[lit("then")] >> term_parser >> omit[lit("else")] >> term_parser;

            term_parser = zero_parser | boolean_parser | succ_parser | pred_parser | iszero_parser | if_parser;
        }

        qi::rule<Iterator, ast::term(), ascii::space_type> term_parser;
        qi::rule<Iterator, ast::zero(), ascii::space_type> zero_parser;
        qi::rule<Iterator, ast::boolean(), ascii::space_type> boolean_parser;
        qi::rule<Iterator, ast::succ_expr(), ascii::space_type> succ_parser;
        qi::rule<Iterator, ast::pred_expr(), ascii::space_type> pred_parser;
        qi::rule<Iterator, ast::iszero_expr(), ascii::space_type> iszero_parser;
        qi::rule<Iterator, ast::if_expr(), ascii::space_type> if_parser;
    };
}

struct evaluator
{
    typedef unsigned result_type;

    result_type operator()(ast::zero const &) const
    {
        return 0u;
    }

    result_type operator()(ast::boolean const &b) const
    {
        return static_cast<unsigned>(b.value);
    }

    result_type operator()(ast::succ_expr const &s) const
    {
        return boost::apply_visitor(*this, s.value) + 1u;
    }

    result_type operator()(ast::pred_expr const &p) const
    {
        return boost::apply_visitor(*this, p.value) - 1u;
    }

    result_type operator()(ast::iszero_expr const &isz) const
    {
        return static_cast<unsigned>(boost::apply_visitor(*this, isz.value) == 0u);
    }

    result_type operator()(ast::if_expr const &ite) const
    {
        if (boost::apply_visitor(*this, ite.condition))
            return boost::apply_visitor(*this, ite.if_true);
        else
            return boost::apply_visitor(*this, ite.if_false);
    }
};

int main()
{
    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input.empty() || input[0] == 'q' || input[0] == 'Q')
            break;

        parser::simple<std::string::const_iterator> grammar;
        boost::spirit::ascii::space_type space;
        ast::term expr;
        std::string::const_iterator first(input.begin());
        std::string::const_iterator last(input.end());
        bool r = boost::spirit::qi::phrase_parse(first, last, grammar, space, expr);
        if (r && first == last)
            std::cout << "result = " << boost::apply_visitor(evaluator(), expr);
        else
            std::cout << "parse failed!";
        std::cout << std::endl;
    }

    return 0;
}
