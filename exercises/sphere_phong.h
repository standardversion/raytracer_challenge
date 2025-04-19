#pragma once
#include "../tuple.h"
#include "../canvas.h"
#include "../colour.h";
#include "../ppm.h"
#include "../intersection.h"
#include "../sphere.h"
#include "../phong.h"

void sphere_phong_exercise()
{
	const tuple_t ray_origin{ tuple_t::point(0, 0, -5) };
	const double wall_z{ 10 };
	const double wall_size{ 7 };
	const double canvas_pixels{ 1000 };
	const double pixel_size{ wall_size / canvas_pixels };
	const double half_wall_size{ wall_size / 2 };
	canvas_t canvas{ (int)canvas_pixels, (int)canvas_pixels };
	auto sphere{ Sphere::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(sphere->material);
	if (phong) {
		phong->colour = { 1, 0.2, 1 };
	}
	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(-10, 10, -10);
	const bool in_shadow{ false };
	for (int y{ 0 }; y < canvas_pixels - 1; y++)
	{
		const double world_y{ half_wall_size - pixel_size * y };
		for (int x{ 0 }; x < canvas_pixels - 1; x++)
		{
			const double world_x{ -half_wall_size + pixel_size * x };
			const tuple_t position{ tuple_t::point(world_x, world_y, wall_z) };
			tuple_t direction{ (position - ray_origin) };
			direction.normalize();
			ray_t ray{ ray_origin, direction };
			intersections_t intersections{};
			sphere->intersect(ray, intersections);
			const intersection_t intersection{ intersections.hit() };
			if (intersection.object != nullptr)
			{
				const tuple_t point_on_sphere{ ray.position(intersection.time) };
				const tuple_t normal{ intersection.object->normal_at(point_on_sphere) };
				const tuple_t eye{ -ray.direction };
				const colour_t pixel_colour{ intersection.object->material->lighting(
					light,
					intersection.object,
					point_on_sphere,
					eye,
					normal,
					in_shadow
				) };
				canvas.write_pixel(x, y, pixel_colour);
			}
		}
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/sphere_phong.ppm");
}