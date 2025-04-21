#pragma once
#include "../tuple.h"
#include "../canvas.h"
#include "../colour.h";
#include "../ppm.h"
#include "../intersection.h"
#include "../sphere.h"
#include "../phong.h"
#include "../world.h"
#include "../camera.h"
#include "../settings.h"
#include "../plane.h"

void plane_exercise()
{
	auto floor{ Plane::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(floor->material);
	if (phong) {
		phong->colour = { 1, 0.9, 0.9 };
		phong->specular = 0;
	}
	//floor->transform = matrix_t::scaling(10, 0.01, 10);

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
	world.add_object(std::make_shared<Light>(light));
	world.add_object(std::move(floor));
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
	ppm.write_to_file("./x64/Release/plane.ppm");
}