#include <string>
#include <vector>
#include <cmath>
#include "gtest/gtest.h"
#include "../settings.h"
#include "../camera.h"
#include "../matrix.h"
#include "../world.h"
#include "../canvas.h"

/*
Scenario: Constructing a camera
  Given hsize ← 160
	And vsize ← 120
	And field_of_view ← π/2
  When c ← camera(hsize, vsize, field_of_view)
  Then c.hsize = 160
	And c.vsize = 120
	And c.field_of_view = π/2
	And c.transform = identity_matrix
*/
TEST(camera, should_construct_a_camera)
{
	int hsize{ 160 };
	int vsize{ 120 };
	double field_of_view{ PI / 2 };
	const Camera c{ hsize, vsize, field_of_view };
	EXPECT_EQ(c.hsize, hsize);
	EXPECT_EQ(c.vsize, vsize);
	EXPECT_EQ(c.field_of_view, field_of_view);
	EXPECT_EQ(c.transform, matrix_t::identity());
}

/*
Scenario: The pixel size for a horizontal canvas
  Given c ← camera(200, 125, π/2)
  Then c.pixel_size = 0.01
*/
TEST(camera, should_pixel_calculate_size_for_a_horizontal_canvas)
{
	int hsize{ 200 };
	int vsize{ 125 };
	double field_of_view{ PI / 2 };
	const Camera c{ hsize, vsize, field_of_view };
	EXPECT_NEAR(c.pixel_size, 0.01, 0.001);
}

/*
Scenario: The pixel size for a vertical canvas
  Given c ← camera(125, 200, π/2)
  Then c.pixel_size = 0.01
*/
TEST(camera, should_pixel_calculate_size_for_a_vertical_canvas)
{
	int hsize{ 125 };
	int vsize{ 200 };
	double field_of_view{ PI / 2 };
	const Camera c{ hsize, vsize, field_of_view };
	EXPECT_NEAR(c.pixel_size, 0.01, 0.001);
}

/*
Scenario: Constructing a ray through the center of the canvas
  Given c ← camera(201, 101, π/2)
  When r ← c.ray_for_pixel(100, 50)
  Then r.origin = point(0, 0, 0)
	And r.direction = vector(0, 0, -1)
*/
TEST(camera, should_constructing_a_ray_through_the_center_of_the_canvas)
{
	double field_of_view{ PI / 2 };
	Camera c{ 201, 101, field_of_view };
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	const tuple_t direction{ tuple_t::vector(0, 0, -1) };
	const ray_t ray{ c.ray_for_pixel(100, 50) };
	EXPECT_EQ(ray.origin, origin);
	EXPECT_EQ(ray.direction, direction);
}

/*
Scenario: Constructing a ray through a corner of the canvas
  Given c ← camera(201, 101, π/2)
  When r ← c.ray_for_pixel(0, 0)
  Then r.origin = point(0, 0, 0)
	And r.direction = vector(0.66519, 0.33259, -0.66851)
*/
TEST(camera, should_constructing_a_ray_through_a_corner_of_the_canvas)
{
	double field_of_view{ PI / 2 };
	Camera c{ 201, 101, field_of_view };
	const tuple_t origin{ tuple_t::point(0, 0, 0) };
	const tuple_t direction{ tuple_t::vector(0.66519, 0.33259, -0.66851) };
	const ray_t ray{ c.ray_for_pixel(0, 0) };
	EXPECT_EQ(ray.origin, origin);
	EXPECT_EQ(ray.direction, direction);
}

/*
Scenario: Constructing a ray when the camera is transformed
  Given c ← camera(201, 101, π/2)
  When c.transform ← rotation_y(π/4) * translation(0, -2, 5)
	And r ← c.ray_for_pixel(100, 50)
  Then r.origin = point(0, 2, -5)
	And r.direction = vector(√2/2, 0, -√2/2)
*/
TEST(camera, should_constructing_a_ray_when_camera_is_transformed)
{
	double field_of_view{ PI / 2 };
	Camera c{ 201, 101, field_of_view };
	c.transform = matrix_t::rotation_y(PI / 4) * matrix_t::translation(0, -2, 5);
	const tuple_t origin{ tuple_t::point(0, 2, -5) };
	const tuple_t direction{ tuple_t::vector(std::sqrt(2) / 2, 0, -std::sqrt(2) / 2)};
	const ray_t ray{ c.ray_for_pixel(100, 50) };
	EXPECT_EQ(ray.origin, origin);
	EXPECT_EQ(ray.direction, direction);
}

/*
Scenario: Rendering a world with a camera
  Given w ← default_world()
	And c ← camera(11, 11, π/2)
	And from ← point(0, 0, -5)
	And to ← point(0, 0, 0)
	And up ← vector(0, 1, 0)
	And c.transform ← view_transform(from, to, up)
  When image ← c.render(w)
  Then pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.2855)
*/
TEST(camera, should_render_a_world_through_a_camera)
{
	World w{ World::default_world() };
	double field_of_view{ PI / 2 };
	Camera c{ 11, 11, field_of_view };
	const tuple_t from{ tuple_t::point(0, 0, -5) };
	const tuple_t to{ tuple_t::point(0, 0, 0) };
	const tuple_t up{ tuple_t::vector(0, 1, 0) };
	c.transform = matrix_t::view_transform(from, to, up);
	const canvas_t canvas{ c.render(w) };
	EXPECT_EQ(canvas.pixel_at(5, 5), colour_t(0.38066, 0.47583, 0.2855));
}