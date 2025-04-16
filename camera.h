#pragma once
#include "scene_object.h"
#include "matrix.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

class Camera : public SceneObject
{
public:
	int hsize;
	int vsize;
	double field_of_view;
	double half_width;
	double half_height;
	double pixel_size;
	matrix_t transform{ matrix_t::identity() };

	Camera(const int hsize, const int vsize, const double fov);
	ray_t ray_for_pixel(const int x, const int y) const;
	canvas_t render(const World& world) const;
};
