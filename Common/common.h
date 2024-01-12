#pragma once

#include <cstdint>
#include <string>

namespace common
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
			Color(uint8_t r, uint8_t g, uint8_t b);

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

	namespace game
	{
		enum class GameState : uint8_t
		{
			NONE,
			PICK_WORD,
			DRAW_AND_GUESS
		};

		enum class PlayerRole : uint8_t
		{
			DRAWING,
			GUESSING,
			NONE
		};

		struct GameSettings
		{
			uint16_t m_roundCount;
			uint16_t m_drawTime;
			uint16_t m_chooseWordOptionCount;

			GameSettings() noexcept;
			GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount) noexcept;

			GameSettings(const GameSettings& other) noexcept = default;
			GameSettings& operator=(const GameSettings& other) noexcept = default;

			GameSettings(GameSettings&& other) noexcept = default;
			GameSettings& operator=(GameSettings&& other) noexcept = default;

		private:
			static const uint16_t s_defaultRoundCount;
			static const uint16_t s_defaultDrawTime;
			static const uint16_t s_defaultChooseWordOptionCount;
		};
	}

	struct Message
	{
		std::string text;
		std::string author;
		uint64_t timestamp;

		Message() = default;
		Message(const std::string& text, const std::string& author, uint64_t timestamp = 0);
		Message(std::string&& text, std::string&& author, uint64_t timestamp = 0);

		Message(const Message& other) = default;
		Message& operator=(const Message& other) = default;
		Message(Message&& other) = default;
		Message& operator=(Message&& other) = default;
	};
}