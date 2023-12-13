#include "commonUtils.h"

utils::img::Color::Color(int32_t rgbHex) :
	r{ uint8_t((rgbHex >> 16) & 0xFF) },
	g{ uint8_t((rgbHex >> 8) & 0xFF) },
	b{ uint8_t(rgbHex & 0xFF) }
{
	/* empty */
}

utils::img::Color::Color(uint32_t rgbHex) :
	Color{ int32_t(rgbHex) }
{
	/* empty */
}

utils::img::Color::Color(int64_t color) :
	Color{ int32_t(color) }
{
	/* empty */
}

int32_t utils::img::Color::ToInt32() const
{
	return r << 16 | g << 8 | b;
}

utils::img::Point::Point(int16_t x, int16_t y, Color color) :
	x{ x },
	y{ y },
	color{ color }
{
	/* empty */
}

utils::img::Point::Point(int16_t x, int16_t y, int32_t color) :
	Point{ x, y, Color{ color } }
{
	/* empty */
}

utils::img::Point::Point(int64_t x, int64_t y, int64_t color) :
	Point{ int16_t(x), int16_t(y), int32_t(color) }
{
	/* empty */
}

utils::img::Update::Update(const Point& point, uint64_t timestamp) :
	point{ point },
	timestamp{ timestamp }
{
	/* empty */
}
