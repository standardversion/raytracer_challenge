#include "light.h"
#include "settings.h"

Light::Light(const colour_t& intensity)
	: SceneObject{}, intensity {
	intensity
}
{ }

tuple_t Light::position() const
{
	return transform * tuple_t::point(0, 0, 0);
}

bool Light::operator==(const Light& l) const
{
	return this->intensity == l.intensity && this->position() == l.position();
}