#include <vector>
#include "bounding_box.h"
#include "cube.h"
#include "intersection.h"

//int bbox_tests = 0;

bbox_t& bbox_t::operator+=(const bbox_t& bb)
{
	add(bb.min);
	add(bb.max);
	return *this;
}

bool bbox_t::contains(const tuple_t& p) const
{
	return (min.x <= p.x && p.x <= max.x) &&
		(min.y <= p.y && p.y <= max.y) &&
		(min.z <= p.z && p.z <= max.z);
}

bool bbox_t::contains(const bbox_t& box) const
{
	return contains(box.min) && contains(box.max);
}

bbox_t bbox_t::transform(const matrix_t& m) const
{
    bbox_t new_box{};  // Initialize a new bounding box to store the transformed coordinates

    // Define the 8 corner points of the original bounding box
    std::vector<tuple_t> points{
        min,  // The minimum corner of the bounding box
        tuple_t::point(min.x, min.y, max.z),  // (min.x, min.y, max.z)
        tuple_t::point(min.x, max.y, min.z),  // (min.x, max.y, min.z)
        tuple_t::point(min.x, max.y, max.z),  // (min.x, max.y, max.z)
        tuple_t::point(max.x, min.y, min.z),  // (max.x, min.y, min.z)
        tuple_t::point(max.x, min.y, max.z),  // (max.x, min.y, max.z)
        tuple_t::point(max.x, max.y, min.z),  // (max.x, max.y, min.z)
        max   // The maximum corner of the bounding box
    };

    // For each corner point, apply the transformation matrix and add the resulting point to the new bounding box
    for (const auto& p : points)
    {
        new_box.add(m * p); 
    }
    return new_box;
}



bool bbox_t::intersect(const ray_t& ray) const
{
	//++bbox_tests;  // Increment the counter to keep track of how many bounding box tests have been performed.

	// Calculate the intersection times for each axis (x, y, z) based on the ray's direction and the bounding box's min/max values.
	const double tmin_x = (min.x - ray.origin.x) * ray.inv_direction.x;
	const double tmax_x = (max.x - ray.origin.x) * ray.inv_direction.x;

	const double tmin_y = (min.y - ray.origin.y) * ray.inv_direction.y;
	const double tmax_y = (max.y - ray.origin.y) * ray.inv_direction.y;

	const double tmin_z = (min.z - ray.origin.z) * ray.inv_direction.z;
	const double tmax_z = (max.z - ray.origin.z) * ray.inv_direction.z;

	// Compute the overall entry (tmin) and exit (tmax) times for the ray along the bounding box.
	// tmin is the latest entry time, and tmax is the earliest exit time.
	const double tmin = std::max(
		std::max(std::min(tmin_x, tmax_x), std::min(tmin_y, tmax_y)),  // Entry time: max of the min intersections along x, y, z axes.
		std::min(tmin_z, tmax_z)   // Entry time: min of the z-axis min/max times.
	);

	const double tmax = std::min(
		std::min(std::max(tmin_x, tmax_x), std::max(tmin_y, tmax_y)),  // Exit time: min of the max intersections along x, y, z axes.
		std::max(tmin_z, tmax_z)   // Exit time: max of the z-axis min/max times.
	);

	// If tmin <= tmax, the ray intersects the bounding box.
	// If tmin > tmax, the ray does not intersect the box (it's outside the box).
	return tmin <= tmax;
}


std::pair<bbox_t, bbox_t> bbox_t::split() const
{
    // Calculate the extent (size) of the bounding box along each axis (x, y, z)
    const double dx{ abs(max.x) + abs(min.x) };  // Total width along x-axis
    const double dy{ abs(max.y) + abs(min.y) };  // Total height along y-axis
    const double dz{ abs(max.z) + abs(min.z) };  // Total depth along z-axis

    // Initialize x0, y0, z0 (minimum corner of the bounding box) and x1, y1, z1 (maximum corner of the bounding box)
    double x0{ min.x };
    double y0{ min.y };
    double z0{ min.z };

    double x1{ max.x };
    double y1{ max.y };
    double z1{ max.z };

    // If the bounding box is not a cube (its extents along x, y, and z are not the same),
    // we determine which axis is the longest and split along that axis.
    if (dx != dy || dx != dz || dy != dz)
    {
        // If the x-axis is the longest dimension, we split along the x-axis
        if (dx > dy && dx > dz)
        {
            x0 = x1 = x0 + dx / 2.0;  // Set x0 and x1 to the middle of the x-axis.
        }
        // If the y-axis is the longest dimension, we split along the y-axis
        else if (dy > dx && dy > dz)
        {
            y0 = y1 = y0 + dy / 2.0;  // Set y0 and y1 to the middle of the y-axis.
        }
        // If the z-axis is the longest dimension, we split along the z-axis
        else
        {
            z0 = z1 = z0 + dz / 2.0;  // Set z0 and z1 to the middle of the z-axis.
        }
    }
    else
    {
        // If the bounding box is a cube (equal extents in all directions),
        // we split along the x-axis by default.
        x0 = x1 = x0 + dx / 2.0;  // Set x0 and x1 to the middle of the x-axis.
    }

    // Calculate the middle point of the bounding box using the updated x0, y0, z0, and x1, y1, z1 values
    tuple_t mid_min{ tuple_t::point(x0, y0, z0) };
    tuple_t mid_max{ tuple_t::point(x1, y1, z1) };

    // Create two new bounding boxes: the left and the right halves of the split bounding box
    const bbox_t left{ min, mid_max };   // The left half is from the original min to the middle point (mid_max)
    const bbox_t right{ mid_min, max };  // The right half is from the middle point (mid_min) to the original max

    // Return the pair of the left and right bounding boxes
    return std::pair<bbox_t, bbox_t>{left, right};
}