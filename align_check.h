#pragma once
#include "pattern.h"
#include "colour.h"

class AlignCheck : public Pattern
{
public:
	colour_t c; 
	colour_t d;
	colour_t e;

	AlignCheck(const colour_t& main, const colour_t& ul, const colour_t& ur, const colour_t& bl, const colour_t& br);
	colour_t at(const tuple_t& point) const override;
	colour_t at(const double u, const double v) const override;
};