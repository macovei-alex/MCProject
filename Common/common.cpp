#include "common.h"

common::img::Color::Color(int64_t color) :
	r{ uint8_t((color >> 16) & 0xFF) },
	g{ uint8_t((color >> 8) & 0xFF) },
	b{ uint8_t(color & 0xFF) }
{
	/* empty */
}

common::img::Color::Color(uint8_t r, uint8_t g, uint8_t b) :
	r{ r },
	g{ g },
	b{ b }
{
	/* empty */
}

int32_t common::img::Color::ToInt32() const
{
	return r << 16 | g << 8 | b;
}

common::img::Point::Point(int64_t x, int64_t y, int64_t color) :
	x{ (int16_t)x },
	y{ (int16_t)y },
	color{ color }
{
	/* empty */
}

common::img::Point::Point(int64_t x, int64_t y, Color color) :
	x{ (int16_t)x },
	y{ (int16_t)y },
	color{ color }
{
	/* empty */
}

common::img::Update::Update(const Point& point, uint64_t timestamp) :
	point{ point },
	timestamp{ timestamp }
{
	/* empty */
}

common::Message::Message(const std::string& text, const std::string& author, uint64_t timestamp) :
	text{ text },
	author{ author },
	timestamp{ timestamp }
{
	/* empty */
}

common::Message::Message(std::string&& text, std::string&& author, uint64_t timestamp) :
	text{ std::move(text) },
	author{ std::move(author) },
	timestamp{ timestamp }
{
	/* empty */
}
