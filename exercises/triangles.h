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

void triangles_exercise()
{
	auto mesh{ Mesh::create(".\\assets\\sphere.obj", true) };
	//auto mesh{ Cube::create() };
	auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
	if (phong) {
		phong->colour = { 1, 1, 1 };
		phong->specular = 0;
		phong->diffuse = 0.8;
		phong->ambient = 1;
		//Checker checker{ { 0, 0, 0 }, { 1, 1, 1 } };
		//checker.transform = matrix_t::scaling(0.25, 0.25, 0.25);
		//phong->pattern = std::make_shared<Checker>(checker);
	}

	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(0, 1.5, -1);
	World world{};
	world.add_object(std::make_shared<Light>(light));
	world.add_object(std::move(mesh));

	Camera camera{ 240, 135, PI / 2 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(0, 0, -5),
		tuple_t::point(0, 0, 0),
		tuple_t::vector(0, 1, 0)
	);
	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/triangle_mesh.ppm");
}