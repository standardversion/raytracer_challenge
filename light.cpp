#include "light.h"
#include "settings.h"
#include "world.h"

Light::Light(const colour_t& intensity, const int usteps, const int vsteps)
	: SceneObject{}, intensity { intensity }, usteps{ usteps }, vsteps{ vsteps }
{ }

tuple_t Light::position() const
{
	return transform * tuple_t::point(0, 0, 0);
}

int Light::get_usteps() const
{
	return usteps;
}

int Light::get_vsteps() const
{
	return vsteps;
}