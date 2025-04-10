#include <iostream>
#include <algorithm>
#include "projectile.h"
#include "environment.h"
#include "settings.h"
#include "tuple.h"
#include "canvas.h"
#include "colour.h";
#include "ppm.h"
#include "matrix.h"

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

int main()
{
	projectile_exercise();
	clock_exercise();
	return 0;
}
