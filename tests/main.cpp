#include <cwt-cucumber/cucumber.hpp>

int main(int argc, char** argv)
{

	cuke::results::test_status status{ cuke::entry_point(argc, const_cast<const char**>(argv)) };

	if (status == cuke::results::test_status::passed)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}