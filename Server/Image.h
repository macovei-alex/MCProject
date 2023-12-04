#pragma once

#include <vector>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color() = default;
	Color(uint32_t color);
};

struct Point
{
	int16_t x;
	int16_t y;
	Color color;
};

struct Update
{
	Point point;
	uint64_t timestamp;
};

class Image
{
public:
	Image() = default;
	Image(const Image& other) = default;
	Image& operator=(const Image& other) = default;
	Image(Image&& other) = default;
	Image& operator=(Image&& other) = default;

public:
	void AddUpdate(const Update& update);
	void AddUpdate(Update&& update);
	std::vector<Update> GetUpdatesAfter(uint64_t timestamp);

private:
	std::vector<Update> m_updates;
};

