#pragma once
#include "../tuple.h"
#include "../canvas.h"
#include "../colour.h";
#include "../ppm.h"
#include "../intersection.h"
#include "../sphere.h"

void sphere_shadow_exercise()
{
	const tuple_t ray_origin{ tuple_t::point(0, 0, -5) };
	const double wall_z{ 10 };
	const double wall_size{ 7 };
	const double canvas_pixels{ 1000 };
	const double pixel_size{ wall_size / canvas_pixels };
	const double half_wall_size{ wall_size / 2 };
	canvas_t canvas{ (int)canvas_pixels, (int)canvas_pixels };
	const colour_t red{ 1, 0, 0 };
	const auto sphere{ Sphere::create() };
	for (int y{ 0 }; y < canvas_pixels - 1; y++)
	{
		const double world_y{ half_wall_size - pixel_size * y };
		for (int x{ 0 }; x < canvas_pixels - 1; x++)
		{
			const double world_x{ -half_wall_size + pixel_size * x };
			const tuple_t position{ tuple_t::point(world_x, world_y, wall_z) };
			tuple_t direction{ (position - ray_origin) };
			direction.normalize();
			const ray_t ray{ ray_origin, direction };
			intersections_t intersections{};
			sphere->intersect(ray, intersections);
			const auto intersection{ intersections.hit() };
			if (intersection && intersection->object != nullptr)
			{
				canvas.write_pixel(x, y, red);
			}
		}
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/sphere_shadow.ppm");
}