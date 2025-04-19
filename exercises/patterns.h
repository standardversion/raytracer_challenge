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
#include "../checker.h"
#include "../ring.h"
#include "../stripe.h"
#include "../gradient.h"

void patterns_exercise()
{
	auto floor{ Plane::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(floor->material);
	if (phong) {
		phong->colour = { 1, 0.9, 0.9 };
		phong->specular = 0;
		const Checker checker{ { 1, 0, 1 }, { 0, .1, 0.75 } };
		phong->pattern = std::make_shared<Checker>(checker);
	}

	auto middle{ Sphere::create() };
	middle->transform = matrix_t::translation(-0.5, 1, 0.5);
	phong = std::dynamic_pointer_cast<Phong>(middle->material);
	if (phong) {
		phong->colour = { 0.1, 1, 0.5 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
		Gradient gradient{ { 0, 0, 1 }, { 1, 1, 0.5 } };
		gradient.transform = matrix_t::rotation_z(PI / 3);
		phong->pattern = std::make_shared<Gradient>(gradient);
	}

	auto right{ Sphere::create() };
	right->transform = matrix_t::translation(1.5, 0.5, -0.5) * matrix_t::scaling(0.5, 0.5, 0.5);
	phong = std::dynamic_pointer_cast<Phong>(right->material);
	if (phong) {
		phong->colour = { 0.5, 1, 0.1 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
		Stripe stripe{ { 0, 1, 1 }, { 1, 1, 0 } };
		stripe.transform = matrix_t::scaling(0.2, 0.2, 0.2);
		phong->pattern = std::make_shared<Stripe>(stripe);
	}

	auto left{ Sphere::create() };
	left->transform = matrix_t::translation(-1.5, 0.33, -0.75) * matrix_t::scaling(0.33, 0.33, 0.33);
	phong = std::dynamic_pointer_cast<Phong>(left->material);
	if (phong) {
		phong->colour = { 1, 0.8, 0.1 };
		phong->diffuse = 0.7;
		phong->specular = 0.3;
		Stripe stripe2{ { 1, 1, 1 }, { 0, 0, 1 } };
		stripe2.transform = matrix_t::scaling(.1, .1, .1) * matrix_t::rotation_z(PI / 4);
		phong->pattern = std::make_shared<Stripe>(stripe2);
	}
	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(-10, 10, -10);
	World world{};
	world.add_object(std::make_unique<Light>(light));
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
	ppm.write_to_file("./x64/Release/patterns.ppm");
}