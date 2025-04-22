#pragma once
#include "triangle.h"

class SmoothTriangle : public Triangle
{
public:
	/** @brief The vertex normal of the triangle at vertex p1. */
	tuple_t n1;
	/** @brief The vertex normal of the triangle at vertex p2. */
	tuple_t n2;
	/** @brief The vertex normal of the triangle at vertex p3. */
	tuple_t n3;

	SmoothTriangle
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
	);

	tuple_t local_normal_at(const tuple_t& local_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const override;
};
