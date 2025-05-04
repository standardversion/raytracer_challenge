#pragma once
#include "vector"
#include "pattern.h"

class CubeMap : public Pattern
{
public:
	CubeMap(
		std::shared_ptr<Pattern> left,
		std::shared_ptr<Pattern> right,
		std::shared_ptr<Pattern> front,
		std::shared_ptr<Pattern> back,
		std::shared_ptr<Pattern> up,
		std::shared_ptr<Pattern> down
	);
	colour_t at(const tuple_t& point) const override;

private:
	std::vector<std::shared_ptr<Pattern>> patterns;
};