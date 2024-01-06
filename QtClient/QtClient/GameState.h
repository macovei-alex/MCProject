#pragma once

#include <cstdint>

enum class GameState : uint8_t
{
	NONE,
	PICK_WORD,
	DRAWING_AND_GUESSING,
};