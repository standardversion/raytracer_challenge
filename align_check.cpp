#include "align_check.h"


AlignCheck::AlignCheck(const colour_t& main, const colour_t& ul, const colour_t& ur, const colour_t& bl, const colour_t& br)
	: Pattern{ main, ul }, c{ ur }, d{ bl }, e{ br }
{

}

colour_t AlignCheck::at(const tuple_t& point) const
{
	return a;
}

colour_t AlignCheck::at(const double u, const double v) const
{
	if (v > 0.8)
	{
		if (u < 0.2)
		{
			return b;
		}
		if (u > 0.8)
		{
			return c;
		}
	}
	else if (v < 0.2)
	{
		if (u < 0.2)
		{
			return d;
		}
		if (u > 0.8)
		{
			return e;
		}
	}
	return a;
}