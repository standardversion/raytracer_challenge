#pragma once
#include "../sphere.h"
#include "../group.h"
#include "../sphere.h"
#include "../cylinder.h"
#include "../settings.h"
#include "../light.h"
#include "../world.h"
#include "../camera.h"
#include "../canvas.h"
#include "../ppm.h"

void hexagon_exercise()
{
	auto hex{ std::make_shared<Group>() };
	World world{};
	for (int n{ 0 }; n < 6; n++)
	{
		auto side{ std::make_shared<Group>() };
		auto s{ Sphere::create() };
		s->transform = matrix_t::translation(0, 0, -1) * matrix_t::scaling(0.25, 0.25, 0.25);
		
		auto e{ Cylinder::create() };
		e->minimum = 0;
		e->maximum = 1;
		e->transform = matrix_t::translation(0, 0, -1) *
			matrix_t::rotation_y(-PI / 6) *
			matrix_t::rotation_z(-PI / 2) *
			matrix_t::scaling(0.25, 1, 0.25);
		side->add(s, e);
		side->transform = matrix_t::rotation_y(n * (PI / 3));
		hex->add(side);
	}
	world.add_object(hex);
	Light light{ colour_t{1, 1, 1} };
	light.transform = matrix_t::translation(0, 1.5, -1);
	
	world.add_object(std::make_shared<Light>(light));
	Camera camera{ 960, 540, PI / 2 };
	camera.transform = matrix_t::view_transform(
		tuple_t::point(3, 2, -5),    // Camera position: right + up + back
		tuple_t::point(0, 0, 0),     // Look at the center of the hexagon
		tuple_t::vector(0, 1, 0)     // Up direction
	);

	canvas_t image{ camera.render(world) };
	const ppm_t ppm{ image };
	ppm.write_to_file("./x64/Release/hex.ppm");
}