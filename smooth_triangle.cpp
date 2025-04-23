#include "smooth_triangle.h"

SmoothTriangle::SmoothTriangle
(
	const tuple_t& v1,
	const tuple_t& v2,
	const tuple_t& v3,
	const std::pair<double, double>& v1_uv,
	const std::pair<double, double>& v2_uv,
	const std::pair<double, double>& v3_uv,
	const tuple_t& n1,
	const tuple_t& n2,
	const tuple_t& n3
)
	: Triangle{ v1, v2, v3, v1_uv, v2_uv, v3_uv }, n1{ n1 }, n2{ n2 }, n3{ n3 }
{}

std::shared_ptr<SmoothTriangle> SmoothTriangle::create(
	const tuple_t& v1,
	const tuple_t& v2,
	const tuple_t& v3,
	const std::pair<double, double>& v1_uv,
	const std::pair<double, double>& v2_uv,
	const std::pair<double, double>& v3_uv,
	const tuple_t& n1,
	const tuple_t& n2,
	const tuple_t& n3
)
{
	return std::make_shared<SmoothTriangle>(v1, v2, v3, v1_uv, v2_uv, v3_uv, n1, n2, n3);
}

tuple_t SmoothTriangle::local_normal_at
(
	const tuple_t& local_point,
	const double alpha,
	const double beta,
	const double gamma
) const
{
	return { n1 * alpha + n2 * beta + n3 * gamma };
}