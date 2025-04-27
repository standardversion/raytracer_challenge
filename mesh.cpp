#include "mesh.h"
#include "triangle.h"
#include "phong.h"
#include "smooth_triangle.h"

Mesh::Mesh() = default;


Mesh::Mesh(const wavefront_t& obj, bool smooth)
	: smooth{ smooth }
{
	if (!obj.vertex_normals_avg.size())
	{
		smooth = false; // no normal data;
	}
	for (const auto& face : obj.faces)
	{
		if (smooth)
		{
			triangles.push_back(
				SmoothTriangle::create(
					obj.vertices.at(face.a - 1),
					obj.vertices.at(face.b - 1),
					obj.vertices.at(face.c - 1),
					obj.uvs.at(face.a_uv - 1),
					obj.uvs.at(face.b_uv - 1),
					obj.uvs.at(face.c_uv - 1),
					obj.vertex_normals_avg.at(face.a - 1),
					obj.vertex_normals_avg.at(face.b - 1),
					obj.vertex_normals_avg.at(face.c - 1)
				) 
			);
		}
		else
		{
			triangles.push_back(
				Triangle::create(
					obj.vertices.at(face.a - 1),
					obj.vertices.at(face.b - 1),
					obj.vertices.at(face.c - 1),
					obj.uvs.at(face.a_uv - 1),
					obj.uvs.at(face.b_uv - 1),
					obj.uvs.at(face.c_uv - 1)
				)
			);
		}
	}
	bbox = bounds();
}

Mesh::Mesh(const char* obj_filename, bool smooth)
	: Mesh{ wavefront_t{obj_filename}, smooth }
{
}

std::shared_ptr<Mesh> Mesh::create(const wavefront_t& obj, bool smooth)
{
	auto mesh{ std::make_shared<Mesh>(obj, smooth) };
	for (auto& tri : mesh->triangles) {
		tri->parent = mesh;
		tri->material = mesh->material;
	}
	return mesh;
}

std::shared_ptr<Mesh> Mesh::create(const char* obj_filename, bool smooth)
{
	auto mesh{ std::make_shared<Mesh>(obj_filename, smooth) };
	auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
	for (auto& tri : mesh->triangles) {
		tri->parent = mesh;
		tri->material = mesh->material;
	}

	return mesh;
}

void Mesh::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	if (bbox.intersect(local_ray))
	{
		for (const auto& triangle : triangles)
		{
			triangle->local_intersect(local_ray, intersections);
		}
	}
}

tuple_t Mesh::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	// just return the input as a normal
	// this should never get called since the object in the intersection will be the
	// triangle which is part of this mesh and not the mesh itself
	return tuple_t::vector(local_point.x, local_point.y, local_point.z);
}

bbox_t Mesh::bounds() const
{
	bbox_t box{};
	for (const auto& triangle : triangles)
	{
		const bbox_t child_box{ triangle->bounds_in_parent_space() };
		box += child_box;
	}
	return box;
}