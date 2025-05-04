#pragma once
#include "canvas.h"
#include "pattern.h"

class PatternFile : public Pattern
{
public:
	const char* file;

	canvas_t canvas;

	PatternFile(const char* filepath);

	colour_t at(const tuple_t& point) const override;

	colour_t at(const double u, const double v) const override;
};
