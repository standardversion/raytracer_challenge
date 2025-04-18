#include "stripe.h"

Stripe::Stripe(const colour_t& a, const colour_t& b)
	: Pattern{ a, b }
{ }

colour_t Stripe::at(const tuple_t& point) const
{
	return ((int)floor(point.x) % 2) ? b : a;
}