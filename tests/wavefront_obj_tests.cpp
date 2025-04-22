#include "gtest/gtest.h"
#include "../wavefront_obj.h"
#include "../tuple.h"

/*
Scenario: Opening a non existant obj file
  Given filepath ← "./foo.obj"
  When w ← wavefront_t(filepath)
  Throws invalid_argument
*/
TEST(wavefront_obj, should_throw_if_trying_to_load_non_existant_obj)
{
	
	EXPECT_THROW(wavefront_t(".\\foo.obj"), std::invalid_argument);
}

/*
Scenario: Reading vertex values from obj file
  Given filepath ← "./assets/cube.obj"
  When w ← wavefront_t(filepath)
  Then w.vertices.size() = 8
	And w.vertices[0] = vector(1.0, 1.0, -1.0)
	And w.vertices[1] = vector(1.0, -1.0, -1.0)
	And w.vertices[2] = vector(1.0, 1.0, 1.0)
	And w.vertices[3] = vector(1.0, -1.0, 1.0)
	And w.vertices[4] = vector(-1.0, 1.0, -1.0)
	And w.vertices[5] = vector(-1.0, -1.0, -1.0)
	And w.vertices[6] = vector(-1.0, 1.0, 1.0)
	And w.vertices[0] = vector(-1.0, -1.0, 1.0)
*/
TEST(wavefront_obj, should_read_vertex_values)
{

	const wavefront_t w{ "..\\..\\tests\\assets\\cube.obj" };
	EXPECT_EQ(w.vertices.size(), 8);
	EXPECT_EQ(w.vertices[0], tuple_t::vector(1.0, 1.0, -1.0));
	EXPECT_EQ(w.vertices[1], tuple_t::vector(1.0, -1.0, -1.0));
	EXPECT_EQ(w.vertices[2], tuple_t::vector(1.0, 1.0, 1.0));
	EXPECT_EQ(w.vertices[3], tuple_t::vector(1.0, -1.0, 1.0));
	EXPECT_EQ(w.vertices[4], tuple_t::vector(-1.0, 1.0, -1.0));
	EXPECT_EQ(w.vertices[5], tuple_t::vector(-1.0, -1.0, -1.0));
	EXPECT_EQ(w.vertices[6], tuple_t::vector(-1.0, 1.0, 1.0));
	EXPECT_EQ(w.vertices[7], tuple_t::vector(-1.0, -1.0, 1.0));
}

/*
Scenario: Reading vertex normal values from obj file
  Given filepath ← "./assets/cube.obj"
  When w ← wavefront_t(filepath)
  Then w.vertex_normals.size() = 8
	And w.vertex_normals[0].size() = 3
	And w.vertex_normals[1].size() = 3
	And w.vertex_normals[2].size() = 3
	And w.vertex_normals[3].size() = 3
	And w.vertex_normals[4].size() = 3
	And w.vertex_normals[5].size() = 3
	And w.vertex_normals[6].size() = 3
	And w.vertex_normals[7].size() = 3
*/
TEST(wavefront_obj, should_read_vertex_normal_values)
{

	const wavefront_t w{ "..\\..\\tests\\assets\\cube.obj" };
	EXPECT_EQ(w.vertex_normals.size(), 8);
	for (const auto& normals : w.vertex_normals)
	{
		EXPECT_EQ(normals.size(), 3);
	}
}

/*
Scenario: Averaging vertex normal values from obj file
  Given filepath ← "./assets/cube.obj"
  When w ← wavefront_t(filepath)
  Then w.vertex_normals_avg.size() = 8
*/
TEST(wavefront_obj, should_avg_vertex_normal_values)
{

	const wavefront_t w{ "..\\..\\tests\\assets\\cube.obj" };
	EXPECT_EQ(w.vertex_normals_avg.size(), 8);
	for (const auto& normals : w.vertex_normals)
	{
		for (const auto& normal : normals)
		{
			for (const auto& avg_normal : w.vertex_normals_avg)
			{
				EXPECT_NE(normal, avg_normal);
			}
		}
	}
}

/*
Scenario: Reading uv values from obj file
  Given filepath ← "./assets/cube.obj"
  When w ← wavefront_t(filepath)
  Then w.uvs.size() = 4
	And w.uvs[0] = (1.0, 0.0)
	And w.uvs[1] = (0.0, 1.0)
	And w.uvs[2] = (0.0, 0.0)
	And w.uvs[3] = (1.0, 1.0)
*/
TEST(wavefront_obj, should_read_uv_values)
{

	const wavefront_t w{ "..\\..\\tests\\assets\\cube.obj" };
	EXPECT_EQ(w.uvs.size(), 4);
	EXPECT_EQ(w.uvs[0], std::make_pair(1.0, 0.0));
	EXPECT_EQ(w.uvs[1], std::make_pair(0.0, 1.0));
	EXPECT_EQ(w.uvs[2], std::make_pair(0.0, 0.0));
	EXPECT_EQ(w.uvs[3], std::make_pair(1.0, 1.0));
}

/*
Scenario: Reading face values from obj file
  Given filepath ← "./assets/cube.obj"
  When w ← wavefront_t(filepath)
  Then w.faces.size() = 12

*/
TEST(wavefront_obj, should_read_face_values)
{

	const wavefront_t w{ "..\\..\\tests\\assets\\cube.obj" };
	EXPECT_EQ(w.faces.size(), 12);
	EXPECT_EQ(w.faces[0].a, 5);
	EXPECT_EQ(w.faces[0].b, 3);
	EXPECT_EQ(w.faces[0].c, 1);
	EXPECT_EQ(w.faces[0].a_uv, 1);
	EXPECT_EQ(w.faces[0].b_uv, 2);
	EXPECT_EQ(w.faces[0].c_uv, 3);
	EXPECT_EQ(w.faces[0].a_normal, 1);
	EXPECT_EQ(w.faces[0].b_normal, 1);
	EXPECT_EQ(w.faces[0].c_normal, 1);
}
