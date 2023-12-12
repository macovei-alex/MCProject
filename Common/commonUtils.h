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
			Color(int32_t color);
			Color(uint32_t color);
			Color(int64_t color);
			int32_t ToInt32() const;
			bool operator==(const Color& other) const = default;
		};

		struct Point
		{
			int16_t x;
			int16_t y;
			Color color;
			bool operator==(const Point& other) const = default;
		};

		struct Update
		{
			Point point;
			uint64_t timestamp;
			bool operator==(const Update& other) const = default;
		};
	}
}