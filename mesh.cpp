#include "mesh.h"
#include "triangle.h"
#include "smooth_triangle.h"

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
				std::make_shared<SmoothTriangle>(
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
				std::make_shared<Triangle>(
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
}

Mesh::Mesh(const char* obj_filename, bool smooth)
	: Mesh{ wavefront_t{obj_filename}, smooth }
{
}

std::shared_ptr<Mesh> Mesh::create(const wavefront_t& obj, bool smooth)
{
	return std::make_shared<Mesh>(obj, smooth);
}

std::shared_ptr<Mesh> Mesh::create(const char* obj_filename, bool smooth)
{
	return std::make_shared<Mesh>(obj_filename, smooth);
}

void Mesh::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	for (const auto& triangle : triangles)
	{
		triangle->local_intersect(local_ray, intersections);
	}
}

tuple_t Mesh::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	return local_point;
}