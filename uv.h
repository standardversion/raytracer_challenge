#pragma once

struct uv_t {
	double u;
	double v;

	bool operator==(const uv_t& c) const;
};
