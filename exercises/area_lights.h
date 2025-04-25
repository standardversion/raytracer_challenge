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
#include "../mesh.h"
#include "../checker.h"
#include "../ring.h"
#include "../stripe.h"
#include "../gradient.h"
#include "../cube.h"
#include "../plane.h"
#include "../area_light.h"
#include "../sequence.h"

void area_lights_exercise()
{

	const tuple_t corner{ tuple_t::point(-1, 2, 4) };
	const tuple_t full_uvec{ tuple_t::vector(2, 0, 0) };
	const tuple_t full_vvec{ tuple_t::vector(0, 2, 0) };
	const int usteps{ 10 };
	const int vsteps{ 10 };
	Sequence sequence{ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 };
	auto key = std::make_shared<AreaLight>(corner, full_uvec, usteps, full_vvec, vsteps, sequence, colour_t{ 1.5, 1.5, 1.5 });

	auto cube{ Cube::create() };
	cube->cast_shadows = false;
	auto cube_material = std::dynamic_pointer_cast<Phong>(cube->material);
	if (cube_material) {
		cube_material->colour = { 1.5, 1.5, 1.5 };
		cube_material->specular = 0;
		cube_material->diffuse = 0;
		cube_material->ambient = 1;
	}

	cube->transform = matrix_t::translation(0, 3, 4) * matrix_t::scaling(1, 1, 0.01);

	auto plane{ Plane::create() };
	auto plane_material = std::dynamic_pointer_cast<Phong>(plane->material);
	if (plane_material) {
		plane_material->colour = { 1, 1, 1 };
		plane_material->specular = 0;
		plane_material->diffuse = 0.67;
		plane_material->ambient = 0.025;
	}

	auto sphere{ Sphere::create() };
	auto sph_material = std::dynamic_pointer_cast<Phong>(sphere->material);
	if (sph_material) {
		sph_material->colour = { 1, 0, 0 };
		sph_material->ambient = 0.1;
		sph_material->diffuse = 0.6;
		sph_material->specular = 0;
		sph_material->reflective = 0.3;
	}
	sphere->transform = matrix_t::translation(0.5, 0.5, 0) * matrix_t::scaling(0.5, 0.5, 0.5);

	auto sphere2{ Sphere::create() };
	auto sph2_material = std::dynamic_pointer_cast<Phong>(sphere2->material);
	if (sph2_material) {
		sph2_material->colour = { 0.5, 0.5, 1 };
		sph2_material->ambient = 0.1;
		sph2_material->diffuse = 0.6;
		sph2_material->specular = 0;
		sph2_material->reflective = 0.3;
	}
	sphere2->transform = matrix_t::translation(-0.25, 0.33, 0) * matrix_t::scaling(0.33, 0.33, 0.33);


	World world{};
	world.add_object(key);
	world.add_object(cube);
	world.add_object(plane);
	world.add_object(sphere);
	world.add_object(sphere2);


	Camera camera{ 400, 160, 0.7854 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(-3, 1, 2.5),
		tuple_t::point(0, 0.5, 0),
		tuple_t::vector(0, 1, 0)
	);

	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/area_light.ppm");
}