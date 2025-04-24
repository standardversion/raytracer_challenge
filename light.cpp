#include "light.h"
#include "settings.h"
#include "world.h"

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

double Light::intensity_at(const tuple_t& point, const World& w) const
{
	return w.is_shadowed(point, position()) ? 0.0 : 1.0;
	
}