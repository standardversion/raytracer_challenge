#include <cmath>
#include "intersection_state.h"
#include "tuple.h"

double intersection_state::schlick() const
{
	// Compute the cosine of the angle between the eye vector and the surface normal
	double cos = tuple_t::dot(eye_vector, normal);

	// If the ray is moving from a denser to a less dense medium, check for total internal reflection
	if (n1 > n2)
	{
		// Compute the ratio of refractive indices
		const double n = n1 / n2;

		// Compute the square of the sine of the transmitted angle using Snell's Law
		const double sin2_t = pow(n, 2) * (1.0 - pow(cos, 2));

		// Total internal reflection occurs if sin2_t > 1
		if (sin2_t > 1.0)
		{
			// All light is reflected
			return 1.0;
		}

		// Otherwise, compute the cosine of the transmitted angle using trigonometric identity
		const double cos_t = std::sqrt(1.0 - sin2_t);

		// When computing reflectance, use the cosine of the transmitted angle
		cos = cos_t;
	}

	// Compute the reflectance at normal incidence (Schlick's base reflectance term)
	const double r0 = pow((n1 - n2) / (n1 + n2), 2);

	// Apply the Schlick approximation formula to estimate reflectance at the given angle
	return r0 + (1 - r0) * pow((1 - cos), 5);
}
