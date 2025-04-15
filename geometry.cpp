#include "geometry.h"

Geometry::Geometry()
    : SceneObject{}
{
	renderable = true;
}

tuple_t Geometry::normal_at(const tuple_t& world_point) const {
    tuple_t local_point = transform.inverse() * world_point;
    tuple_t local_normal = local_normal_at(local_point);
    tuple_t world_normal = transform.inverse().transpose() * local_normal;
    world_normal.w = 0;
    world_normal.normalize();
    return world_normal;
}