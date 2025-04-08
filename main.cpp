#include <iostream>
#include <algorithm>
#include "projectile.h"
#include "environment.h"
#include "tuple.h"
#include "canvas.h"
#include "colour.h";
#include "ppm.h"

projectile_t update(const projectile_t& proj, const environment_t& env)
{
	tuple_t position{ proj.position + proj.velocity };
	tuple_t velocity{ proj.velocity + env.gravity + env.wind };
	return {
		position,
		velocity
	};
}

int main()
{
	bool is_running{ true };
	projectile_t proj{ tuple_t::point(0, 1, 0), tuple_t::normalize(tuple_t::vector(1, 1, 0)) * 11.25 };
	environment_t env{ tuple_t::vector(0, -0.1, 0), tuple_t::vector(-0.01, 0, 0) };
	canvas_t canvas{ 1200, 500 };
	const colour_t red{ 1, 0, 0 };
	int counter{ 1 };
	while (proj.position.y >= 0)
	{
		proj = update(proj, env);
		std::cout << "Projectile position is X:" << proj.position.x << " Y: " << proj.position.y << " Z: " << proj.position.z << " ; At tick " << counter << " " << std::endl;
		counter++;
		int x{ (int)std::round(proj.position.x) };
		int y{ (int)std::round(canvas.height - proj.position.y) };
		canvas.write_pixel(x, y, red);
	}
	const ppm_t ppm{ canvas };
	ppm.write_to_file("./x64/Release/projectile.ppm");
	return 0;
}
