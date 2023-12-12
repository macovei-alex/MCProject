#include "commonUtils.h"

utils::img::Color::Color(int32_t rgbHex) :
	r{ uint8_t((rgbHex >> 16) & 0xFF) },
	g{ uint8_t((rgbHex >> 8) & 0xFF) },
	b{ uint8_t(rgbHex & 0xFF) }
{
	/* empty */
}

utils::img::Color::Color(uint32_t rgbHex) :
	Color(int32_t(rgbHex))
{
	/* empty */
}

utils::img::Color::Color(int64_t color) :
	Color(int32_t(color))
{
	/* empty */
}

int32_t utils::img::Color::ToInt32() const
{
	return r << 16 | g << 8 | b;
}
