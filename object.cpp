#include "object.h"

Object::Object()
	: id{ obj_id_counter++ }
{
	transform = matrix_t::identity();
}

tuple_t Object::normal_at(const tuple_t& world_point) const {
    tuple_t local_point = transform.inverse() * world_point;
    tuple_t local_normal = local_normal_at(local_point);
    tuple_t world_normal = transform.inverse().transpose() * local_normal;
    world_normal.w = 0;
    world_normal.normalize();
    return world_normal;
}