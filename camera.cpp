#include <cmath>
#include <chrono>
#include <iostream>
#include "camera.h"
#include "settings.h"

Camera::Camera(const int hsize, const int vsize, const double fov)
	: SceneObject{}, hsize{ hsize }, vsize{ vsize }, field_of_view{ fov }
{
	double half_view{ tan(field_of_view / 2) }; // since image plane is 1 unit away
	double aspect{ (hsize / (double)vsize) };
	if (aspect >= 1)
	{
		half_width = half_view;
		half_height = half_view / aspect;
	}
	else
	{
		half_width = half_view * aspect;
		half_height = half_view;
	}
	pixel_size = (half_width * 2) / hsize;
}

ray_t Camera::ray_for_pixel(const int x, const int y) const
{
	// the offset from the edge of the canvas to the pixel's center
	double x_offset{ (x + 0.5) * pixel_size };
	double y_offset{ (y + 0.5) * pixel_size };
	// the untransformed coordinates of the pixel in world space.
	// (remember that the camera looks toward -z, so +x is to the *left*.)
	double world_x{ half_width - x_offset };
	double world_y{ half_height - y_offset };
	// using the camera matrix, transform the canvas point and the origin,
	// and then compute the ray's direction vector.
	// (remember that the canvas is at z=-1)
	tuple_t pixel{ transform.inverse() * tuple_t::point(world_x, world_y, -1) };
	tuple_t origin{ transform.inverse() * tuple_t::point(0, 0, 0) };
	tuple_t direction{ pixel - origin };
	direction.normalize();
	return { origin, direction };
}


///////////////////////////////////////////////////////////////////////////////
// Process the graphics pipeline stages for a raytracer
///////////////////////////////////////////////////////////////////////////////
// +---------------+
// | Scene Setup   |  <-- define geometry, materials, lights, camera
// +---------------+
//       |
//       v
// +-------------------+
// | Ray Generation    |  <-- shoot rays from camera through image plane
// +-------------------+
//       |
//       v
// +---------------------+
// | Ray-Scene Intersection |  <-- find nearest object hit by each ray
// +---------------------+
//       |
//       v
// +-------------------+
// | Shading / Lighting |  <-- compute color using material, lights, etc.
// +-------------------+
//       |
//       v
// +----------------+
// | Secondary Rays |  <-- spawn reflection, refraction, shadow rays
// +----------------+
//       |
//       v
// +--------------------+
// | Recursive Tracing  |  <-- trace secondary rays, accumulate color
// +--------------------+
//       |
//       v
// +------------------+
// | Image Composition |  <-- write final pixel color to image
// +------------------+
///////////////////////////////////////////////////////////////////////////////

canvas_t Camera::render(const World& world) const
{
	canvas_t image{ hsize, vsize };
	auto start = std::chrono::high_resolution_clock::now();
	for (int y{ 0 }; y < vsize - 1; y++)
	{
		for (int x{ 0 }; x < hsize - 1; x++)
		{
			const ray_t ray{ ray_for_pixel(x, y) };
			const colour_t colour{ world.colour_at(ray, MAX_REFLECTION_DEPTH) };
			image.write_pixel(x, y, colour);
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::cout << "Render time: " << duration.count() << " seconds\n";
	return image;
}