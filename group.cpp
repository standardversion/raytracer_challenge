#include "group.h"
#include "geometry.h"


std::shared_ptr<SceneObject> Group::find(const SceneObject* obj) const
{
	for (const auto& child : children) {
		if (child.get() == obj) {
			return child;
		}
	}
	return nullptr;
}

void Group::intersect(const ray_t& ray, intersections_t& intersections) const
{
	if (children.empty())
	{
		return;
	}
	const ray_t transformed_ray{ ray.transform(transform.inverse()) };
	for (const auto& child : children)
	{
		if (auto geo{dynamic_pointer_cast<Geometry>(child)})
		{
			geo->intersect(transformed_ray, intersections);
		}
		else if (auto grp = std::dynamic_pointer_cast<Group>(child))
		{
			grp->intersect(transformed_ray, intersections);
		}
	}
}

bbox_t Group::bounds() const
{
	bbox_t box{};
	for (const auto& child : children)
	{
		if (const auto geo{ dynamic_pointer_cast<Geometry>(child) })
		{
			const bbox_t child_box{geo->bounds_in_parent_space()};
			box += child_box;
		}
	}
	return box;
}