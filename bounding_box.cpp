#include "bounding_box.h"

bbox_t& bbox_t::operator+=(const bbox_t& bb)
{
	return *this;
}