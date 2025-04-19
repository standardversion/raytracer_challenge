#pragma once
#include "../tuple.h"
#include "../canvas.h"
#include "../ppm.h"

/**
 * @brief Represents the physical environment in which objects move.
 *
 * Typically used in physics simulations to model the effects of gravity and wind
 * on projectiles or particles in a scene.
 */
struct environment_t
{
    /**
     * @brief The gravitational force vector applied to objects in the environment.
     *
     * Usually points downward (e.g., (0, -9.8, 0)) to simulate Earth's gravity.
     */
    tuple_t gravity{ tuple_t::vector(0, 0, 0) };

    /**
     * @brief The wind force vector applied to objects in the environment.
     *
     * Can be used to simulate directional air resistance or drag.
     */
    tuple_t wind{ tuple_t::vector(0, 0, 0) };
};

/**
 * @brief Represents a moving object affected by forces like gravity and wind.
 *
 * Commonly used in basic physics simulations to model projectile motion
 * over time under the influence of an environment.
 */
struct projectile_t
{
    /**
     * @brief The current position of the projectile in world space.
     *
     * Typically updated every frame or timestep based on velocity and forces.
     */
    tuple_t position{ tuple_t::point(0, 0, 0) };

    /**
     * @brief The current velocity vector of the projectile.
     *
     * Determines the direction and speed of the projectile's motion.
     * Affected by environmental forces like gravity and wind.
     */
    tuple_t velocity{ tuple_t::vector(0, 0, 0) };
};


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