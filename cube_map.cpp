#include "cube.h"
#include "cube_map.h"
#include "uv.h"
#include "pattern.h"

CubeMap::CubeMap(
	std::shared_ptr<Pattern> left,
	std::shared_ptr<Pattern> right,
	std::shared_ptr<Pattern> front,
	std::shared_ptr<Pattern> back,
	std::shared_ptr<Pattern> up,
	std::shared_ptr<Pattern> down
)
	: patterns{ left, right, front, back, up, down }
{

}

colour_t CubeMap::at(const tuple_t& point) const
{
	const Cube_Faces face{ Cube::face_from_point(point) };
	const uv_t uv{ Cube::static_get_uv(point) };
	return patterns[face]->at(uv.u, uv.v);
}