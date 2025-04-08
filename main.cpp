#include <iostream>
#include "projectile.h"
#include "environment.h"
#include "tuple.h"

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
	projectile_t proj{ tuple_t::point(0, 1, 0), tuple_t::normalize(tuple_t::vector(1, 1, 0)) };
	environment_t env{ tuple_t::vector(0, -0.1, 0), tuple_t::vector(-0.01, 0, 0) };
	int counter{ 1 };
	while (proj.position.y >= 0)
	{
		proj = update(proj, env);
		std::cout << "Projectile position is X:" << proj.position.x << " Y: " << proj.position.y << " Z: " << proj.position.z << " ; At tick " << counter << " " << std::endl;
		counter++;
	}
	return 0;
}