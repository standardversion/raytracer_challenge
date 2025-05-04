#include "uv.h"

bool uv_t::operator==(const uv_t& uv) const
{
	return u == uv.u && v == uv.v;
}