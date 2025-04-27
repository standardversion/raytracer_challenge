#include "gtest/gtest.h"
#include "../mesh.h"
#include "../wavefront_obj.h"
#include "../phong.h"
#include "../smooth_triangle.h"
#include "../intersection.h"
#include "../bounding_box.h"

/*
Scenario: Create mesh from obj file path
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
  When m ← mesh(filepath,false)
  Then m.triangles.size = 2
	And m.smooth = true
	And m.bbox.min = point(-1, -1, 1)
	And m.bbox.max = point(1, 1, 1)
*/
TEST(mesh, should_load_mesh_from_obj_filepath)
{
	const auto m{ Mesh::create("..\\..\\tests\\assets\\face.obj", true) };
	EXPECT_EQ(m->triangles.size(), 2);
	EXPECT_EQ(m->smooth, true);
	EXPECT_EQ(m->bbox.min, tuple_t::point(-1, -1, 1));
	EXPECT_EQ(m->bbox.max, tuple_t::point(1, 1, 1));
}

/*
Scenario: Create mesh wavefront obj object
  Given w ← wavefront("..\\..\\tests\\assets\\face.obj")
  When m ← mesh(w)
  Then m.triangles.size = 2
	And m.smooth = false
	And m.bbox.min = point(-1, -1, 1)
	And m.bbox.max = point(1, 1, 1)
*/
TEST(mesh, should_load_mesh_from_wavefront_obj)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	const auto m{ Mesh::create(w, false) };
	EXPECT_EQ(m->triangles.size(), 2);
	EXPECT_EQ(m->smooth, false);
	EXPECT_EQ(m->bbox.min, tuple_t::point(-1, -1, 1));
	EXPECT_EQ(m->bbox.max, tuple_t::point(1, 1, 1));
}

/*
Scenario: Create mesh with smooth triangles
  Given w ← wavefront("..\\..\\tests\\assets\\face.obj")
  When m ← mesh(w, true)
  Then m.triangles[0].n1 = vector(0.0 0.0 1.0)
	And m.triangles[0].n2 = vector(0.0 0.0 1.0)
	And m.triangles[0].n3 = vector(0.0 0.0 1.0)
	And m.triangles[1].n1 = vector(0.0 0.0 1.0)
	And m.triangles[1].n2 = vector(0.0 0.0 1.0)
	And m.triangles[1].n3 = vector(0.0 0.0 1.0)
*/
TEST(mesh, should_create_mesh_consisting_of_smooth_triangles)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	const auto m{ Mesh::create(w, true) };
	auto t1{ std::dynamic_pointer_cast<SmoothTriangle>(m->triangles[0]) };
	auto t2{ std::dynamic_pointer_cast<SmoothTriangle>(m->triangles[1]) };
	EXPECT_EQ(t1->n1, tuple_t::vector(0.0, 0.0, 1.0));
	EXPECT_EQ(t1->n2, tuple_t::vector(0.0, 0.0, 1.0));
	EXPECT_EQ(t1->n3, tuple_t::vector(0.0, 0.0, 1.0));
	EXPECT_EQ(t2->n1, tuple_t::vector(0.0, 0.0, 1.0));
	EXPECT_EQ(t2->n2, tuple_t::vector(0.0, 0.0, 1.0));
	EXPECT_EQ(t2->n3, tuple_t::vector(0.0, 0.0, 1.0));
}

/*
Scenario: Assign mesh material to triangles
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
	And m ← mesh(filepath)
	And m.material.ambient = 1
  Then m.triangles[0].material.ambient = 1
	And m.triangles[1].material.ambient = 1
*/
TEST(mesh, should_assign_mesh_material_to_triangles)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	auto m{ Mesh::create(w, false) };
	auto phong = std::dynamic_pointer_cast<Phong>(m->material);
	if (phong) {
		phong->ambient = 1;
	}
	auto p1{ std::dynamic_pointer_cast<Phong>(m->triangles[0]->material) };
	auto p2{ std::dynamic_pointer_cast<Phong>(m->triangles[1]->material) };
	EXPECT_EQ(phong->ambient, p1->ambient);
	EXPECT_EQ(phong->ambient, p2->ambient);
}

/*
Scenario: A ray misses all triangles in the mesh
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
	And m ← mesh(filepath)
	And r ← ray(point(0, 10, 0), vector(0, 0, 1))
  When xs ← local_intersect(t, m)
  Then xs is empty
*/
TEST(mesh, should_miss_triangles_in_mesh)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	auto m{ Mesh::create(w, false) };
	const ray_t r{ tuple_t::point(0, 10, 0), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	m->local_intersect(r, i);
	EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray strikes a triangle in the mesh
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
	And m ← mesh(filepath)
	And r ← ray(point(0, 10, 0), vector(0, 0, 1))
  When xs ← local_intersect(t, m)
  Then xs is empty
*/
TEST(mesh, should_hit_a_triangle_in_mesh)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	auto m{ Mesh::create(w, false) };
	const ray_t r{ tuple_t::point(0.5, 0, 0), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	m->local_intersect(r, i);
	EXPECT_EQ(i.entries.size(), 1);
	EXPECT_EQ(i[0].time, 1);
}

/*
Scenario: Mesh local_normal_at function should return input point as vector
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
	And m ← mesh(filepath)
	And p ← point(0, 1, 0)
  When n ← m.local_normal_at(p)
  Then n = normal(p)
*/
TEST(mesh, should_not_do_anything_in_loca_normal_at_func)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	auto m{ Mesh::create(w, false) };
	const tuple_t p{ tuple_t::point(0, 1, 0) };
	intersections_t i{};
	EXPECT_EQ(m->local_normal_at(p), tuple_t::vector(0, 1, 0));
}

/*
Scenario: A mesh has a bounding box that contains all its triangles
  Given p1 ← point(1, 1, 1)
	And p2 ← point(3, 1, 1)
	And p3 ← point(2, 4, 1)
	And p4 ← point(-2, -1, 0)
	And p5 ← point(-1, 2, 0)
	And p6 ← point(0, -1, 0)
	And t1 ← triangle(p1, p2, p3)
	And t2 ← triangle(p4, p5, p6)
	And mesh ← mesh()
	And add_triangle(mesh, t1)
	And add_triangle(mesh, t2)
  When box ← bounds_of(mesh)
  Then box.min = point(-2, -1, 0)
	And box.max = point(3, 4, 1)
*/
TEST(mesh, should_have_bounding_box)
{
	Mesh m{};
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(1, 1, 1),
			tuple_t::point(3, 1, 1),
			tuple_t::point(2, 4, 1)
		)
	);
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(-2, -1, 0),
			tuple_t::point(-1, 2, 0),
			tuple_t::point(0, -1, 0)
		)
	);
	const bbox_t box{ m.bounds() };
	EXPECT_EQ(box.min, tuple_t::point(-2, -1, 0));
	EXPECT_EQ(box.max, tuple_t::point(3, 4, 1));
}

/*
Scenario: A ray misses the bounding box of the mesh
  Given p1 ← point(1, 1, 1)
	And p2 ← point(3, 1, 1)
	And p3 ← point(2, 4, 1)
	And p4 ← point(-2, -1, 0)
	And p5 ← point(-1, 2, 0)
	And p6 ← point(0, -1, 0)
	And t1 ← triangle(p1, p2, p3)
	And t2 ← triangle(p4, p5, p6)
	And mesh ← mesh()
	And add_triangle(mesh, t1)
	And add_triangle(mesh, t2)
	And box ← bounds_of(mesh)
  When r ← ray(point(5, 5, -2), vector(1, 0, 0))
  Then intersects(box, r) is false
*/
TEST(mesh, should_miss_mesh_if_ray_does_not_intersect_bbox)
{
	Mesh m{};
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(1, 1, 1),
			tuple_t::point(3, 1, 1),
			tuple_t::point(2, 4, 1)
		)
	);
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(-2, -1, 0),
			tuple_t::point(-1, 2, 0),
			tuple_t::point(0, -1, 0)
		)
	);
	m.bbox = m.bounds();
	const ray_t r{ tuple_t::point(5, 5, -2), tuple_t::vector(1, 0, 0) };
	intersections_t i{};
	m.local_intersect(r, i);
	EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray hits the bounding box of the mesh but misses the triangles
  Given p1 ← point(1, 1, 1)
	And p2 ← point(3, 1, 1)
	And p3 ← point(2, 4, 1)
	And p4 ← point(-2, -1, 0)
	And p5 ← point(-1, 2, 0)
	And p6 ← point(0, -1, 0)
	And t1 ← triangle(p1, p2, p3)
	And t2 ← triangle(p4, p5, p6)
	And mesh ← mesh()
	And add_triangle(mesh, t1)
	And add_triangle(mesh, t2)
	And box ← bounds_of(mesh)
  When r ← ray(point(0, 0, -5), vector(0, 0, 1))
  Then intersects(box, r) is false
*/
TEST(mesh, should_hit_mesh_if_ray_intersects_bbox_but_misses_triangles)
{
	Mesh m{};
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(1, 1, 1),
			tuple_t::point(3, 1, 1),
			tuple_t::point(2, 4, 1)
		)
	);
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(-2, -1, 0),
			tuple_t::point(-1, 2, 0),
			tuple_t::point(0, -1, 0)
		)
	);
	m.bbox = m.bounds();
	const ray_t r{ tuple_t::point(0, 0, -5), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	m.local_intersect(r, i);
	EXPECT_EQ(i.entries.size(), 0);
}

/*
Scenario: A ray hits the bounding box of the mesh but hit the triangles
  Given p1 ← point(1, 1, 1)
	And p2 ← point(3, 1, 1)
	And p3 ← point(2, 4, 1)
	And p4 ← point(-2, -1, 0)
	And p5 ← point(-1, 2, 0)
	And p6 ← point(0, -1, 0)
	And t1 ← triangle(p1, p2, p3)
	And t2 ← triangle(p4, p5, p6)
	And mesh ← mesh()
	And add_triangle(mesh, t1)
	And add_triangle(mesh, t2)
	And box ← bounds_of(mesh)
  When r ← ray(point(-1, 0, -2), vector(0, 0, 1))
  Then intersects(box, r) is false
*/
TEST(mesh, should_hit_mesh_if_ray_intersects_bbox_and_intersects_triangles)
{
	Mesh m{};
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(1, 1, 1),
			tuple_t::point(3, 1, 1),
			tuple_t::point(2, 4, 1)
		)
	);
	m.triangles.push_back(
		Triangle::create(
			tuple_t::point(-2, -1, 0),
			tuple_t::point(-1, 2, 0),
			tuple_t::point(0, -1, 0)
		)
	);
	m.bbox = m.bounds();
	const ray_t r{ tuple_t::point(-1, 0, -2), tuple_t::vector(0, 0, 1) };
	intersections_t i{};
	m.local_intersect(r, i);
	EXPECT_EQ(i.entries.size(), 1);
}