#include "scene_object.h"

SceneObject::SceneObject()
    : id{ sceneobj_id_counter++ }
{
    transform = matrix_t::identity();
}
