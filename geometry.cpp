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

tuple_t Geometry::normal_at(const tuple_t& world_point) const {
    tuple_t local_point = transform.inverse() * world_point;
    tuple_t local_normal = local_normal_at(local_point);
    tuple_t world_normal = transform.inverse().transpose() * local_normal;
    world_normal.w = 0;
    world_normal.normalize();
    return world_normal;
}