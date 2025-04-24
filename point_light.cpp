#include "point_light.h"
#include "settings.h"
#include "world.h"

PointLight::PointLight(const colour_t& intensity)
	: Light{intensity}
{
}


bool PointLight::operator==(const Light& l) const
{
	const PointLight* other = dynamic_cast<const PointLight*>(&l);
	if (!other) return false; // Not the same type

	return *this == *other;
	
}

bool PointLight::operator==(const PointLight& l) const
{
	return this->intensity == l.intensity && this->position() == l.position();
}

double PointLight::intensity_at(const tuple_t& point, const World& w) const
{
	return w.is_shadowed(point, position()) ? 0.0 : 1.0;

}