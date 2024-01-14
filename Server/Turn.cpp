#include "Turn.h"

#include <thread>
#include <numeric>
#include <algorithm>
#include <iostream>

Turn::Turn(uint8_t turnNumber) :
	m_turnNumber{ turnNumber },
	m_word{},
	m_showLetterIDs{},
	m_playStartTime{ chr::system_clock::time_point::max() }
{
	/* empty */
}

Turn::Turn(const Turn& other) :
	m_turnNumber{ other.m_turnNumber.load() },
	m_word{ other.m_word.Get() },
	m_showLetterIDs{ std::move(other.m_showLetterIDs.Get()) },
	m_playStartTime{ other.m_playStartTime.Get() }
{
	/* empty */
}

Turn& Turn::operator=(const Turn& other)
{
	if (this == &other)
		return *this;
	m_turnNumber = other.m_turnNumber.load();
	m_word = other.m_word.Get();
	m_showLetterIDs = std::move(other.m_showLetterIDs.Get());
	m_playStartTime = other.m_playStartTime.Get();

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
	return chr::duration_cast<chr::seconds>(chr::system_clock::now() - m_playStartTime.Get());
}

std::string Turn::GetWord() const noexcept
{
	return m_word.Get();
}

void Turn::SetWord(const std::string& word) noexcept
{
	m_word = word;
}

void Turn::SetWord(std::string&& word) noexcept
{
	m_word = std::move(word);
}

void Turn::Reset(utils::ThreadSafe<std::vector<Player>>& players, size_t drawingPlayerID)
{
	Reset(players, players.GetRef()[drawingPlayerID]);
}

void Turn::Reset(utils::ThreadSafe<std::vector<Player>>& players, Player& drawingPlayer)
{
	m_turnNumber++;
	m_word.Set("");

	{
		LOCK(players.GetMutex());

		std::ranges::for_each(players.GetRef(), [](Player& player) {
			player.SetGuessStatus(false);
			player.ResetCurrentScore();
			player.SetGameRole(common::game::PlayerRole::GUESSING);
			});

		drawingPlayer.SetGuessStatus(true);
		drawingPlayer.SetGameRole(common::game::PlayerRole::DRAWING);
	}

	std::cout << "Turn reset" << std::endl;
}

void Turn::Start(const utils::ThreadSafe<std::vector<Player>>& players, chr::seconds drawingTime, bool& m_stopped)
{
	std::cout << "Turn started" << std::endl;

	m_playStartTime = chr::system_clock::now();
	do
	{
		std::this_thread::sleep_for(chr::seconds{ 1 });

		{
			/*std::lock_guard<std::mutex> lock{ players.GetMutex() };

			if (std::ranges::all_of(players.GetRef(), [](const Player& player) {
				return player.GetGuessStatus();
				}))
			{
				std::cout << "All players guessed the word" << std::endl;
				break;
			}*/
		}

	} while (!m_stopped && chr::system_clock::now() - m_playStartTime.Get() < drawingTime);

	std::cout << "Turn over" << std::endl;
}