#include <stdexcept>
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

tuple_t Geometry::normal_at(const tuple_t& world_point, const double alpha, const double beta, const double gamma) const {
    tuple_t local_point = world_to_object(world_point);
    tuple_t local_normal = local_normal_at(local_point, alpha, beta, gamma);
    return normal_to_world(local_normal);
}

bbox_t Geometry::bounds_in_parent_space() const
{
    return bounds().transform(transform);
}

uv_t Geometry::get_uv(const tuple_t& point) const
{
    throw std::runtime_error("UV mapping not supported");
}
