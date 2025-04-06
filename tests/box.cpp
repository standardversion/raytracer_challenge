#include <string>
#include <vector>
#include <cwt-cucumber/cucumber.hpp>
#include <cwt-cucumber/asserts.hpp>
#include <cwt-cucumber/context.hpp>
#include <cwt-cucumber/defines.hpp>
#include <cwt-cucumber/get_args.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../box.h"

GIVEN(init_box, "An empty box")
{
	const box& my_box = cuke::context<box>();
	cuke::equal(my_box.items_count(), 0);
}
WHEN(add_item, "I place {int} x {string} in it")
{
	const std::size_t count = CUKE_ARG(1);
	const std::string item = CUKE_ARG(2);

	cuke::context<box>().add_items(item, count);
}
THEN(check_box_size, "The box contains {int} item(s)")
{
	const int items_count = CUKE_ARG(1);
	const box& my_box = cuke::context<box>();
	cuke::equal(my_box.items_count(), items_count);
}