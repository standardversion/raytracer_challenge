#include "group.h"
#include "geometry.h"


SceneObject* Group::find(SceneObject* obj)
{
	auto found{ std::find(children.begin(), children.end(), obj) };
	if (found == children.end())
	{
		return nullptr;
	}
	else
	{
		return obj;
	}
}

void Group::intersect(const ray_t& ray, intersections_t& intersections) const
{
	if (children.empty())
	{
		return;
	}
	const ray_t transformed_ray{ ray.transform(transform.inverse()) };
	for (const auto obj : children)
	{
		if (auto geo{dynamic_cast<Geometry*>(obj)})
		{
			geo->intersect(transformed_ray, intersections);
		}
	}
}