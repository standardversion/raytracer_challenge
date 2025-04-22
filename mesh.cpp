#include "mesh.h"
#include "triangle.h"
#include "smooth_triangle.h"

Mesh::Mesh(const wavefront_t& obj)
	:vertices{ obj.vertices },
	vertex_normals{ obj.vertex_normals },
	vertex_normals_avg{ obj.vertex_normals_avg },
	uvs{ obj.uvs },
	faces{ obj.faces }
{

}

Mesh::Mesh(const char* obj_filename)
{
	const wavefront_t obj{ obj_filename };
	vertices = obj.vertices;
	vertex_normals = obj.vertex_normals;
	vertex_normals_avg = obj.vertex_normals_avg;
	uvs = obj.uvs;
	faces = obj.faces;
}

std::shared_ptr<Mesh> Mesh::create(const wavefront_t& obj)
{
	return std::make_shared<Mesh>(obj);
}

std::shared_ptr<Mesh> Mesh::create(const char* obj_filename)
{
	return std::make_shared<Mesh>(obj_filename);
}

void Mesh::local_intersect(const ray_t& local_ray, intersections_t& intersections) const
{
	for (const auto& face : faces)
	{
		if (smooth)
		{
			const SmoothTriangle tri{
				vertices[face.a - 1],
				vertices[face.b - 1],
				vertices[face.c - 1],
				uvs[face.a_uv - 1],
				uvs[face.b_uv - 1],
				uvs[face.c_uv - 1],
				vertex_normals_avg[face.a_normal - 1],
				vertex_normals_avg[face.b_normal - 1],
				vertex_normals_avg[face.c_normal - 1]
			};
			tri.local_intersect(local_ray, intersections);
		}
		else
		{
			const Triangle tri{
				vertices[face.a - 1],
				vertices[face.b - 1],
				vertices[face.c - 1],
				uvs[face.a_uv - 1],
				uvs[face.b_uv - 1],
				uvs[face.c_uv - 1]
			};
			tri.local_intersect(local_ray, intersections);
		}
	}
}

tuple_t Mesh::local_normal_at(const tuple_t& local_point, const double alpha, const double beta, const double gamma) const
{
	return local_point;
}