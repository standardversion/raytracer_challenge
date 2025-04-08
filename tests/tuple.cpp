#include <cwt-cucumber/cucumber.hpp>
#include <cwt-cucumber/asserts.hpp>
#include <cwt-cucumber/context.hpp>
#include <cwt-cucumber/defines.hpp>
#include <cwt-cucumber/get_args.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../tuple.h"
#include <iostream>
#include <string>

CUSTOM_PARAMETER(custom, "{4doubles}", R"(\(.*\))",
	"four doubles")
{
	std::string input{CUKE_PARAM_ARG(1).to_string()};
	std::cout << input << std::endl;
	return input;
}

GIVEN(construct_tuple, "a tuple{4doubles}")
{
	/*const double x{ CUKE_ARG(1) };
	const double y{ CUKE_ARG(2) };
	const double z{ CUKE_ARG(3) };
	const double w{ CUKE_ARG(4) };*/

	std::string event = CUKE_ARG(1);
	std::cout << event << std::endl;
	//const tuple_t& my_tuple{ cuke::context<tuple_t>(x, y, z, w) };
}
THEN(check_x, "a.x is {double}")
{
	const double expected_x{ CUKE_ARG(1) };
	const double actual_x{ cuke::context<tuple_t>().x};
	cuke::equal(actual_x, expected_x);
}