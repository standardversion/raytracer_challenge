#include "gtest/gtest.h"
#include "../mesh.h"
#include "../wavefront_obj.h"
#include "../phong.h"
#include "../smooth_triangle.h"
#include "../intersection.h"

/*
Scenario: Create mesh from obj file path
  Given filepath ← "..\\..\\tests\\assets\\face.obj"
  When m ← mesh(filepath,false)
  Then m.triangles.size = 2
	And m.smooth = true
*/
TEST(mesh, should_load_mesh_from_obj_filepath)
{
	const auto m{ Mesh::create("..\\..\\tests\\assets\\face.obj", false) };
	EXPECT_EQ(m->triangles.size(), 2);
	EXPECT_EQ(m->smooth, false);
}

/*
Scenario: Create mesh wavefront obj object
  Given w ← wavefront("..\\..\\tests\\assets\\face.obj")
  When m ← mesh(w)
  Then m.triangles.size = 2
	And m.smooth = true
*/
TEST(mesh, should_load_mesh_from_wavefront_obj)
{
	const wavefront_t w{ "..\\..\\tests\\assets\\face.obj" };
	const auto m{ Mesh::create(w, false) };
	EXPECT_EQ(m->triangles.size(), 2);
	EXPECT_EQ(m->smooth, false);
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