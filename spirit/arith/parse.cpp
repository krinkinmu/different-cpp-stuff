#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


#include <iostream>
#include <string>
#include <list>

namespace pt
{
	struct nil { };
	struct expression;

	typedef boost::variant<
				nil,
				unsigned,
				boost::recursive_wrapper<expression>
			>
	operand;

	struct binary
	{
		char sign_;
		operand operand_;
	};

	struct expression
	{
		operand first_;
		std::list<binary> rest_;
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	pt::binary,
	(char, sign_)
	(pt::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
	pt::expression,
	(pt::operand, first_)
	(std::list<pt::binary>, rest_)
)

namespace parser {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	struct err_handler
	{
		template <typename, typename, typename>
		struct result { typedef void type; };

		template <typename Iterator>
		void operator()(qi::info const & what, Iterator pos, Iterator last) const
		{
			std::cout << "Error! Expecting " << what
				<< " here: \"" << std::string(pos, last)
				<< "\"" << std::endl;
		}
	};

	phoenix::function<err_handler> const error_handler = err_handler();

	template <typename Iterator>
	struct simple : qi::grammar<Iterator, pt::expression(), ascii::space_type>
	{
		simple() : simple::base_type(expression)
		{
			qi::char_type char_;
			qi::uint_type uint_;
			qi::_2_type _2;
			qi::_3_type _3;
			qi::_4_type _4;

			expression = term >> *((char_('+') > term) | (char_('-') > term));
			term = factor >> *((char_('*') > factor | char_('/') > factor));
			factor = uint_ | ('(' > expression > ')');

			BOOST_SPIRIT_DEBUG_NODE(expression);
			BOOST_SPIRIT_DEBUG_NODE(term);
			BOOST_SPIRIT_DEBUG_NODE(factor);

			qi::on_error<qi::fail>(expression, error_handler(_4, _3, _2));
		}

		qi::rule<Iterator, pt::expression(), ascii::space_type> expression;
		qi::rule<Iterator, pt::expression(), ascii::space_type> term;
		qi::rule<Iterator, pt::operand(), ascii::space_type> factor;
	};
}

namespace ast
{
	struct anode
	{
		virtual ~anode() { }
		virtual void print() const = 0;
	};

	class number_node : public anode
	{
	unsigned num_;

	public:
		number_node(unsigned num) : num_(num) { }

		virtual ~number_node() { }
		virtual void print() const { std::cout << num_; }
	};

	class binary_node : public anode
	{
	boost::shared_ptr<anode> left_;
	boost::shared_ptr<anode> right_;
	char op_;

	public:
		binary_node(boost::shared_ptr<anode> l, boost::shared_ptr<anode> r, char op)
		: left_(l), right_(r), op_(op)
		{ }

		virtual ~binary_node() { }
		virtual void print() const
		{
			std::cout << "(";
			left_->print();
			std::cout << op_;
			right_->print();
			std::cout << ")";
		}
	};

	struct builder
	{
		typedef boost::shared_ptr<anode> result_type;

		result_type operator()(pt::nil) const { BOOST_ASSERT(0); return result_type(); }
		result_type operator()(unsigned num) const { return boost::make_shared<number_node>(num); }
		result_type operator()(pt::expression const & e) const
		{
			boost::shared_ptr<anode> result = boost::apply_visitor(*this, e.first_);
			for (std::list<pt::binary>::const_iterator it(e.rest_.begin()); it != e.rest_.end(); ++it)
				result = boost::make_shared<binary_node>(result, boost::apply_visitor(*this, it->operand_), it->sign_);
			return result;
		}
	};
}

int main()
{
	std::string input;
	while (std::getline(std::cin, input))
	{
		if (input.empty() || input[0] == 'q' || input[0] == 'Q')
			break;

		parser::simple<std::string::const_iterator> grammar;
		pt::expression expr;
		boost::shared_ptr<ast::anode> root;
		boost::spirit::ascii::space_type space;

		std::string::const_iterator first(input.begin());
		std::string::const_iterator last(input.end());
		bool r = boost::spirit::qi::phrase_parse(first, last, grammar, space, expr);
		if (r && first == last)
			ast::builder()(expr)->print();
		else
			std::cout << "parse failed!";
		std::cout << std::endl;
	}

	return 0;
}
