#include "scene_object.h"

SceneObject::SceneObject()
    : id{ sceneobj_id_counter++ }
{
    transform = matrix_t::identity();
}

tuple_t SceneObject::world_to_object(const tuple_t& point) const
{
    tuple_t out_point{ point };
    if (parent)
    {
        out_point = parent->world_to_object(out_point);
    }
    return transform.inverse() * out_point;
}

tuple_t SceneObject::normal_to_world(const tuple_t& normal) const
{
    tuple_t out_normal{ transform.inverse().transpose() * normal };
    out_normal.w = 0;
    out_normal.normalize();
    if (parent)
    {
        out_normal = parent->normal_to_world(out_normal);
    }
    return out_normal;
}