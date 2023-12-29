#pragma once

#include <cstdint>

namespace utils
{
	namespace img
	{
		struct Color
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;

			Color() = default;
			Color(int64_t color);
			Color(const Color& color) = default;
			Color& operator=(const Color& other) = default;
			bool operator==(const Color& other) const = default;
			int32_t ToInt32() const;
		};

		struct Point
		{
			int16_t x;
			int16_t y;
			Color color;

			Point() = default;
			Point(int64_t x, int64_t y, int64_t color);
			Point(int64_t x, int64_t y, Color color);
			Point(const Point& other) = default;
			Point& operator=(const Point& other) = default;
			bool operator==(const Point& other) const = default;
		};

		struct Update
		{
			Point point;
			uint64_t timestamp;

			Update() = default;
			Update(const Point& point, uint64_t timestamp);
			Update(const Update& other) = default;
			Update& operator=(const Update& other) = default;
			bool operator==(const Update& other) const = default;
		};
	}
}