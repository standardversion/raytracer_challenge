#include "geometry.h"
#include "phong.h"

Geometry::Geometry()
    : SceneObject{}
{
	renderable = true;
    material = std::make_shared<Phong>();
}

void Geometry::intersect(const ray_t& ray, intersections_t& intersections) const
{
    const ray_t local_ray{ ray.transform(transform.inverse()) };
    local_intersect(local_ray, intersections);
}

tuple_t Geometry::normal_at(const tuple_t& world_point, const double alpha = 0, const double beta = 0, const double gamma = 0) const {
    tuple_t local_point = world_to_object(world_point);
    tuple_t local_normal = local_normal_at(local_point, alpha, beta, gamma);
    return normal_to_world(local_normal);
}