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
#include "../point_light.h"

void bvh_exercise()
{
	auto mesh{ Mesh::create(".\\assets\\crab.obj", false) };

	// Position the jet with a nice angle
	mesh->transform = matrix_t::rotation_y(-PI / 6) *
		matrix_t::rotation_x(-PI / 16) *
		matrix_t::translation(0, 0.5, 0);

	// Neutral gray material
	auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
	if (phong) {
		phong->colour = { 1, 0.4, 0.4 };
		phong->specular = 0.1;
		phong->diffuse = 0.8;
		phong->ambient = 0.1;
	}

	// === Lights ===
	auto key = std::make_shared<PointLight>(colour_t{ 1.0, 0.95, 0.9 });
	key->transform = matrix_t::translation(-4, 5, -4);

	auto fill = std::make_shared<PointLight>(colour_t{ 0.4, 0.4, 0.5 });
	fill->transform = matrix_t::translation(3, 1, -2);

	auto rim = std::make_shared<PointLight>(colour_t{ 0.8, 0.8, 1.0 });
	rim->transform = matrix_t::translation(0, 3, 5);

	// === Ground Plane ===
	auto plane = Plane::create();
	auto plane_material = std::dynamic_pointer_cast<Phong>(plane->material);
	if (plane_material) {
		plane_material->colour = { 0.5, 0.5, 0.5 };  // Neutral tarmac look
		plane_material->specular = 0;
		plane_material->diffuse = 0.9;
		plane_material->ambient = 0.1;
	}

	// === Sky Background (Gradient Pattern) ===
	auto sky = Plane::create();
	auto sky_material = std::dynamic_pointer_cast<Phong>(sky->material);
	if (sky_material) {
		auto gradient = std::make_shared<Gradient>(
			colour_t{ 0.7, 0.85, 1.0 },  // light blue top
			colour_t{ 1.0, 1.0, 1.0 }    // white bottom
		);
		gradient->transform = matrix_t::scaling(1, 5, 1) *
			matrix_t::rotation_x(PI / 2);
		sky_material->pattern = gradient;
		sky_material->ambient = 1;
		sky_material->diffuse = 0;
		sky_material->specular = 0;
	}
	sky->transform = matrix_t::translation(0, 5, 0); // behind jet

	// === World Setup ===
	World world{};
	world.add_object(key);
	world.add_object(fill);
	world.add_object(rim);
	world.add_object(std::move(mesh));
	//world.add_object(plane); // ground
	//world.add_object(sky);   // background sky

	// === Camera ===
	Camera camera{ 320, 180, PI / 3 };
	//Camera camera{ 960, 540, PI / 3 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(0, 1.5, -5),
		tuple_t::point(0, 0.1, 0),
		tuple_t::vector(0, 1, 0)
	);

	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/bvh.ppm");
}