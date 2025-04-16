#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>
#include "projectile.h"
#include "environment.h"
#include "settings.h"
#include "tuple.h"
#include "canvas.h"
#include "colour.h";
#include "ppm.h"
#include "matrix.h"
#include "sphere.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "light.h"
#include "phong.h"
#include "world.h"
#include "camera.h"

projectile_t update_projectile(const projectile_t& proj, const environment_t& env)
{
	tuple_t position{ proj.position + proj.velocity };
	tuple_t velocity{ proj.velocity + env.gravity + env.wind };
	return {
		position,
		velocity
	};
}

void projectile_exercise()
{
	bool is_running{ true };
	projectile_t proj{ tuple_t::point(0, 1, 0), tuple_t::normalize(tuple_t::vector(1, 1, 0)) * 11.25 };
	environment_t env{ tuple_t::vector(0, -0.1, 0), tuple_t::vector(-0.01, 0, 0) };
	canvas_t canvas{ 1200, 500 };
	const colour_t red{ 1, 0, 0 };
	int counter{ 1 };
	while (proj.position.y >= 0)
	{
		proj = update_projectile(proj, env);
		std::cout << "Projectile position is X:" << proj.position.x << " Y: " << proj.position.y << " Z: " << proj.position.z << " ; At tick " << counter << " " << std::endl;
		counter++;
		int x{ (int)std::round(proj.position.x) };
		int y{ (int)std::round(canvas.height - proj.position.y) };
		canvas.write_pixel(x, y, red);
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/projectile.ppm");
}

void clock_exercise()
{
	canvas_t canvas{ 400, 400 };
	const constexpr double clock_radius{ 400 * 0.25 };
	tuple_t point{ tuple_t::point(0, 0, 1) };
	const colour_t red{ 1, 0, 0 };
	for (int x{ 0 }; x < 12; x++)
	{
		const matrix_t rotation{ matrix_t::rotation_y((x + 1) * (PI / 6)) };
		tuple_t hour_point{ rotation * point };
		double x_coord{ (hour_point.x * clock_radius) + canvas.width / 2 };
		double y_coord{ (hour_point.z * clock_radius) + canvas.width / 2 };
		std::cout << x + 1 << " o'clock X: " << (int)std::round(x_coord) << " Y:" << (int)std::round(y_coord) << std::endl;
		canvas.write_pixel((int)std::round(x_coord), (int)std::round(y_coord), red);
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/clock.ppm");
}

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
			const intersection_t intersection{ intersections.hit() };
			if (intersection.object != nullptr)
			{
				canvas.write_pixel(x, y, red);
			}
		}
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/sphere_shadow.ppm");
}

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
				const colour_t pixel_colour{ intersection.object->material->lighting(light, point_on_sphere, eye, normal) };
				canvas.write_pixel(x, y, pixel_colour);
			}
		}
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/sphere_phong.ppm");
}

void build_scene_exercise()
{
	auto floor{ Sphere::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(floor->material);
	if (phong) {
		phong->colour = { 1, 0.9, 0.9 };
		phong->specular = 0;
	}
	floor->transform = matrix_t::scaling(10, 0.01, 10);

	auto left_wall{ Sphere::create() };
	phong = std::dynamic_pointer_cast<Phong>(left_wall->material);
	if (phong) {
		phong->colour = { 1, 0.9, 0.9 };
		phong->specular = 0;
	}
	left_wall->transform = matrix_t::translation(0, 0, 5) * matrix_t::rotation_y(-PI/4) * matrix_t::rotation_x(PI / 2) *
		matrix_t::scaling(10, 0.01, 10);

	auto right_wall{ Sphere::create() };
	phong = std::dynamic_pointer_cast<Phong>(right_wall->material);
	if (phong) {
		phong->colour = { 1, 0.9, 0.9 };
		phong->specular = 0;
	}
	right_wall->transform = matrix_t::translation(0, 0, 5) * matrix_t::rotation_y(PI / 4) * matrix_t::rotation_x(-PI / 2) *
		matrix_t::scaling(10, 0.01, 10);

	auto middle{ Sphere::create() };
	middle->transform = matrix_t::translation(-0.5, 1, 0.5);
	phong = std::dynamic_pointer_cast<Phong>(middle->material);
	if (phong) {
		phong->colour = { 0.1, 1, 0.5 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
	}

	auto right{ Sphere::create() };
	right->transform = matrix_t::translation(1.5, 0.5, -0.5) * matrix_t::scaling(0.5, 0.5, 0.5);
	phong = std::dynamic_pointer_cast<Phong>(right->material);
	if (phong) {
		phong->colour = { 0.5, 1, 0.1 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
	}

	auto left{ Sphere::create() };
	left->transform = matrix_t::translation(-1.5, 0.33, -0.75) * matrix_t::scaling(0.33, 0.33, 0.33);
	phong = std::dynamic_pointer_cast<Phong>(left->material);
	if (phong) {
		phong->colour = { 1, 0.8, 0.1 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
	}
	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(-10, 10, -10);
	World world{};
	world.add_object(std::make_unique<Light>(light));
	world.add_object(std::move(floor));
	world.add_object(std::move(left_wall));
	world.add_object(std::move(right_wall));
	world.add_object(std::move(middle));
	world.add_object(std::move(right));
	world.add_object(std::move(left));

	Camera camera{ 1000, 500, PI / 3 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(0, 1.5, -5),
		tuple_t::point(0, 1, 0),
		tuple_t::vector(0, 1, 0)
	);
	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/build_scene.ppm");
}

int main()
{
	projectile_exercise();
	clock_exercise();
	sphere_shadow_exercise();
	sphere_phong_exercise();
	return 0;
}
