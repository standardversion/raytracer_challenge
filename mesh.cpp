#include "mesh.h"
#include "triangle.h"
#include "phong.h"
#include "bvh.h"

Mesh::Mesh() = default;


Mesh::Mesh(const wavefront_t& obj, bool smooth)
	: smooth{ smooth }
{
	for (const auto& face : obj.faces)
	{
		std::shared_ptr<Triangle> tri{ Triangle::create(
					obj.vertices.at(face.a - 1),
					obj.vertices.at(face.b - 1),
					obj.vertices.at(face.c - 1)
				) };
		if (face.has_uvs())
		{
			tri->v1_uv = obj.uvs.at(face.a_uv.value() - 1);
			tri->v2_uv = obj.uvs.at(face.b_uv.value() - 1);
			tri->v3_uv = obj.uvs.at(face.c_uv.value() - 1);
			tri->has_uvs = true;
		}
		if (face.has_normals() && smooth)
		{
			tri->n1 = obj.vertex_normals_avg.at(face.a - 1);
			tri->n2 = obj.vertex_normals_avg.at(face.b - 1);
			tri->n3 = obj.vertex_normals_avg.at(face.c - 1);
		}
		triangles.push_back(tri);
	}
	bbox = bounds();
}

Mesh::Mesh(const char* obj_filename, bool smooth)
	: Mesh{ wavefront_t{obj_filename}, smooth }
{
}

std::shared_ptr<Mesh> Mesh::create(const wavefront_t& obj, bool smooth, int bvh_threshold)
{
	auto mesh{ std::make_shared<Mesh>(obj, smooth) };
	for (auto& tri : mesh->triangles) {
		tri->parent = mesh;
		tri->material = mesh->material;
	}
	mesh->create_bvh(bvh_threshold);
	return mesh;
}

std::shared_ptr<Mesh> Mesh::create(const char* obj_filename, bool smooth, int bvh_threshold)
{
	auto mesh{ std::make_shared<Mesh>(obj_filename, smooth) };
	auto phong = std::dynamic_pointer_cast<Phong>(mesh->material);
	for (auto& tri : mesh->triangles) {
		tri->parent = mesh;
		tri->material = mesh->material;
	}
	mesh->create_bvh(bvh_threshold);
	return mesh;
}

void Mesh::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	if (bvh)
	{
		bvh->local_intersect(local_ray, intersections);
	}
	else if (bbox.intersect(local_ray))
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

void Mesh::create_bvh(int threshold) {
	if (triangles.size() <= threshold) return;

	bvh = std::make_unique<bvh_t>();
	for (const auto& tri : triangles) {
		bvh->add(tri);
	}
	bvh->build(threshold);
}
