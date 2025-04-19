#include <cmath>
#include "intersection_state.h"
#include "tuple.h"

double intersection_state::schlick() const
{
	double cos{ tuple_t::dot(eye_vector, normal) };
	if (n1 > n2)
	{
		const double n{ n1 / n2 };
		const double sin2_t{ pow(n, 2) * (1.0 - pow(cos, 2)) };
		if (sin2_t > 1.0)
		{
			return 1.0;
		}
		const double cos_t{ std::sqrt(1.0 - sin2_t) };
		cos = cos_t;
	}
	const double r0{ pow((n1 - n2) / (n1 + n2), 2) };
	return r0 + (1 - r0) * pow((1 - cos), 5);
}