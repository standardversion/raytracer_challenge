#pragma once
#include "colour.h"
#include "pattern.h"

class Gradient : public Pattern
{
public:
    Gradient(const colour_t& a, const colour_t& b);

    colour_t at(const tuple_t& point) const;
};

