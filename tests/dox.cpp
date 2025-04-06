#include <string>
#include <vector>
#include <cwt-cucumber/cucumber.hpp>
#include <cwt-cucumber/asserts.hpp>
#include <cwt-cucumber/context.hpp>
#include <cwt-cucumber/defines.hpp>
#include <cwt-cucumber/get_args.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../dox.h"

GIVEN(init_box, "An empty dox")
{
	const dox& my_dox = cuke::context<dox>();
	cuke::equal(my_dox.items_count(), 0);
}
WHEN(add_item, "I place {int} x {string} in the dox")
{
	const std::size_t count = CUKE_ARG(1);
	const std::string item = CUKE_ARG(2);

	cuke::context<dox>().add_items(item, count);
}
THEN(check_box_size, "The dox contains {int} item(s)")
{
	const int items_count = CUKE_ARG(1);
	const dox& my_dox = cuke::context<dox>();
	cuke::equal(my_dox.items_count(), items_count);
}