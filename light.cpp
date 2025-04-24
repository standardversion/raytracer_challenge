#include "light.h"
#include "settings.h"
#include "world.h"

Light::Light(const colour_t& intensity)
	: SceneObject{}, intensity { intensity }
{ }

tuple_t Light::position() const
{
	return transform * tuple_t::point(0, 0, 0);
}
