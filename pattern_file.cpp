#include "ppm.h"
#include "canvas.h"
#include "pattern_file.h"


PatternFile::PatternFile(const char* filepath)
	: file{filepath}, canvas {canvas_t{ ppm_t{ filepath } }
}
{

}

bool PatternFile::supports_uv() const
{
	return true;
}

colour_t PatternFile::at(const tuple_t& point) const
{
	return colour_t{ 0, 0, 0 };
}

colour_t PatternFile::at(const double u, const double v) const
{
	int x = static_cast<int>(u * (canvas.width - 1) + 0.5);
	int y = static_cast<int>((1 - v) * (canvas.height - 1) + 0.5);
	return canvas.pixel_at(x, y);

}