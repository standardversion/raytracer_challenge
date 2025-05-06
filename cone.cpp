#include <cmath>

#include <algorithm>
#include "cone.h"
#include "intersection.h"
#include "settings.h"


std::shared_ptr<Cone> Cone::create()
{
	return std::make_shared<Cone>();
}

void Cone::intersect_caps(const ray_t& local_ray, intersections_t& intersections) const
{
	// If the cone is not closed or if the ray is parallel to the Y-axis, skip cap intersections
	if (!closed || abs(local_ray.direction.y) < EPSILON)
	{
		return;
	}

	// Check for intersection with the bottom cap (minimum height) of the cone
	double t{ (minimum - local_ray.origin.y) / local_ray.direction.y };
	if (check_cap(local_ray, t, minimum))
	{
		intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}

	// Check for intersection with the top cap (maximum height) of the cone
	t = (maximum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t, maximum))
	{
		intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
	}
}

bool Cone::check_cap(const ray_t& local_ray, const double time, const double height) const
{
	// Calculate the x and z coordinates of the intersection point at 'time'
	const double x{ local_ray.origin.x + time * local_ray.direction.x };
	const double z{ local_ray.origin.z + time * local_ray.direction.z };

	// The cap is a circle with radius based on the height, so check if the point is within that circle
	return (pow(x, 2) + pow(z, 2)) <= pow(height, 2);
}


void Cone::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
    // Calculate the coefficients for the quadratic equation of the cone
    const double a{ pow(local_ray.direction.x, 2) - pow(local_ray.direction.y, 2) + pow(local_ray.direction.z, 2) };
    const double b{
        2 * local_ray.origin.x * local_ray.direction.x
        - 2 * local_ray.origin.y * local_ray.direction.y
        + 2 * local_ray.origin.z * local_ray.direction.z
    };
    const double c{ pow(local_ray.origin.x, 2) - pow(local_ray.origin.y, 2) + pow(local_ray.origin.z, 2) };

    // If both 'a' and 'b' are zero, the ray is parallel to the cone and doesn't intersect it
    if (a == 0 && b == 0)
    {
        return;
    }

    // If 'a' is zero, the intersection is a simple linear equation
    if (a == 0 && abs(b) > 0)
    {
        double t{ -c / (2 * b) };  // Solve the linear equation for t
        intersections.add(t, std::static_pointer_cast<const Geometry>(shared_from_this()));
    }

    // If 'a' is non-zero, solve the quadratic equation for intersections
    if (abs(a) >= EPSILON)
    {
        const double discriminant{ abs(pow(b, 2) - 4 * a * c) };
        if (discriminant >= 0)
        {
            double t0{ (-b - std::sqrt(discriminant)) / (2 * a) };
            double t1{ (-b + std::sqrt(discriminant)) / (2 * a) };

            // Ensure t0 is the smaller value (closer intersection)
            if (t0 > t1)
            {
                std::swap(t0, t1);
            }

            // Check if the intersection is within the valid Y-range (between minimum and maximum heights)
            double y0{ local_ray.origin.y + t0 * local_ray.direction.y };
            if (minimum < y0 && y0 < maximum)
            {
                intersections.add(t0, std::static_pointer_cast<const Geometry>(shared_from_this()));
            }

            double y1{ local_ray.origin.y + t1 * local_ray.direction.y };
            if (minimum < y1 && y1 < maximum)
            {
                intersections.add(t1, std::static_pointer_cast<const Geometry>(shared_from_this()));
            }
        }
    }

    // Check for intersections with the caps
    intersect_caps(local_ray, intersections);
    return;
}

tuple_t Cone::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
    const double distance{ pow(local_point.x, 2) + pow(local_point.z, 2) };

    // If the point is at the maximum height of the cone, the normal is simply (0, 1, 0)
    if (distance < 1 && local_point.y >= maximum - EPSILON)
    {
        return tuple_t::vector(0, 1, 0);
    }
    // If the point is at the minimum height of the cone, the normal is simply (0, -1, 0)
    else if (distance < 1 && local_point.y <= minimum + EPSILON)
    {
        return tuple_t::vector(0, -1, 0);
    }
    else
    {
        // Calculate the normal based on the cone’s surface (the normal vector lies along the cone's slant)
        double y{ std::sqrt(distance) };
        if (local_point.y > 0)
        {
            y = -y;  // Flip the direction if the point is on the upper half of the cone
        }
        return tuple_t::vector(local_point.x, y, local_point.z);
    }
}


bbox_t Cone::bounds() const
{
	const double a{ abs(minimum) };
	const double b{ abs(maximum) };
	const double limit{ std::max(a, b) };
	return bbox_t{ tuple_t::point(-limit, minimum, -limit), tuple_t::point(limit, maximum, limit) };
}