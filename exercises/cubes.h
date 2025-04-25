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
#include "../cube.h"
#include "../checker.h"
#include "../ring.h"
#include "../stripe.h"
#include "../gradient.h"
#include "../point_light.h"

void cubes_exercise()
{
	auto cube{ Cube::create() };
	cube->transform = matrix_t::scaling(4, 4, 4);
	auto phong = std::dynamic_pointer_cast<Phong>(cube->material);
	if (phong) {
		phong->colour = { 1, 1, 1 };
		phong->specular = 0;
		phong->diffuse = 0;
		phong->ambient = .75;
		Checker checker{ { 0, 0, 0 }, { 1, 1, 1 } };
		checker.transform = matrix_t::scaling(0.25, 0.25, 0.25);
		phong->pattern = std::make_shared<Checker>(checker);
	}

	auto sphere{ Sphere::create() };
	//sphere->transform = matrix_t::scaling(0.5, 0.5, 0.5);
	phong = std::dynamic_pointer_cast<Phong>(sphere->material);
	if (phong) {
		phong->colour = { 0.1, 0.1, 0.1 };
		phong->diffuse = 0.6;
		phong->specular = 0.9;
		phong->transparency = 0.8;
		phong->reflective = 0.6;
		phong->refractive_index = 1.5;
	}

	PointLight light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(0, 1.5, -1);
	World world{};
	world.add_object(std::make_shared<Light>(light));
	world.add_object(std::move(cube));
	world.add_object(std::move(sphere));

	Camera camera{ 960, 540, PI / 2 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(0, 0, -2),    // Camera position inside the cube, behind the sphere
		tuple_t::point(0, 0, 0),     // Looking at the center of the cube
		tuple_t::vector(0, 1, 0)     // Up is Y-axis
	);
	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/cube.ppm");
}