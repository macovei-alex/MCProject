#include "Turn.h"

#include <thread>
#include <numeric>
#include <algorithm>

Turn::Turn(uint8_t turnNumber) :
	m_turnNumber{ turnNumber },
	m_word{},
	m_showLetterIDs{},
	m_playStartTime{ chr::system_clock::time_point::max() }
{
	/* empty */
}

Turn::Turn(const Turn& other) :
	m_turnNumber{ other.m_turnNumber },
	m_word{ other.m_word },
	m_showLetterIDs{ other.m_showLetterIDs },
	m_playStartTime{ other.m_playStartTime }
{
	/* empty */
}

Turn& Turn::operator=(const Turn& other)
{
	if (this == &other)
		return *this;
	m_turnNumber = other.m_turnNumber;
	m_word = other.m_word;
	m_showLetterIDs = other.m_showLetterIDs;
	m_playStartTime = other.m_playStartTime;

	return *this;
}

uint8_t Turn::GetTurnNumber() const noexcept
{
	return m_turnNumber;
}

void Turn::SetTurnNumber(uint8_t turnNumber) noexcept
{
	m_turnNumber = turnNumber;
}

chr::seconds Turn::GetTimer() const noexcept
{
	return chr::duration_cast<chr::seconds>(chr::system_clock::now() - m_playStartTime);
}

std::string Turn::GetWord() const noexcept
{
	return m_word;
}

void Turn::SetWord(const std::string& word) noexcept
{
	m_word = word;
}

void Turn::SetWord(std::string&& word) noexcept
{
	m_word = std::move(word);
}

void Turn::SetPlayersMutex(std::shared_ptr<std::mutex> playersMutex) noexcept
{
	m_playersMutex = playersMutex;
}

void Turn::Reset(std::vector<Player>& players, size_t drawingPlayerID)
{
	Reset(players, players[drawingPlayerID]);
}

void Turn::Reset(std::vector<Player>& players, Player& drawingPlayer)
{
	m_turnNumber++;
	m_word = "";
	m_playStartTime = chr::system_clock::now();

	{
		std::lock_guard<std::mutex> lock{ *m_playersMutex };

		std::ranges::for_each(players, [](Player& player) {
			player.SetGuessStatus(false);
			player.ResetCurrentScore();
			player.SetGameRole(common::game::PlayerRole::GUESSING);
			});

		drawingPlayer.SetGameRole(common::game::PlayerRole::DRAWING);
	}
}

void Turn::Start(const std::vector<Player>& players, chr::seconds drawingTime, bool& m_stopped)
{
	m_playStartTime = chr::system_clock::now();
	do
	{
		std::this_thread::sleep_for(chr::seconds{ 1 });

		{
			std::lock_guard<std::mutex> lock{ *m_playersMutex };

			if (std::ranges::all_of(players, [](const Player& player) {
				return player.GetGuessStatus();
				}))
				break;

			size_t count{ static_cast<size_t>(std::ranges::count_if(players, [](const Player& player) {
				return player.GetGuessStatus();
				})) };

			if (count == players.size() - 1)
				break;
		}

	} while (!m_stopped && chr::system_clock::now() - m_playStartTime < drawingTime);
}