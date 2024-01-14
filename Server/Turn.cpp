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
		drawingPlayer.CalculateScoreDrawingPlayer(0, 0, 0);
		drawingPlayer.SetGameRole(common::game::PlayerRole::DRAWING);
	}

	std::cout << "Turn reset" << std::endl;
}

void Turn::Start(utils::ThreadSafe<std::vector<Player>>& players, chr::seconds drawingTime, bool& m_stopped)
{
	std::cout << "Turn started" << std::endl;

	m_playStartTime = chr::system_clock::now();
	uint32_t maxSeconds{ static_cast<uint32_t>(drawingTime.count()) };
	uint16_t drawingPlayerSeconds{ 0 };
	size_t playersGuessedCount{ 1 };

	auto currentTimeLambda{ [this]() -> chr::system_clock::duration {
			return chr::system_clock::now() - m_playStartTime.Get();
		} };

	auto currentSecondsLambda{ [this, &currentTimeLambda]() -> uint16_t {
			return static_cast<uint16_t>(chr::duration_cast<chr::seconds>(
				currentTimeLambda()).count());
		} };

	auto drawingPlayer{ *std::ranges::find_if(players.GetRef(), [](const Player& player) {
			return player.GetRole() == common::game::PlayerRole::DRAWING;
		}) };

	std::cout << "Drawing player initial score:\n";
	std::cout << drawingPlayer.GetName() << " score: " << drawingPlayer.GetCurrentScore() << ' ' << drawingPlayer.GetScore() << std::endl;

	std::cout << "Guessing players initial scores:\n";
	std::ranges::for_each(players.GetRef(), [](Player& player) {
		if (player.GetRole() == common::game::PlayerRole::GUESSING)
			std::cout << player.GetName() << " score: " << player.GetCurrentScore() << ' ' << player.GetScore() << std::endl;
		});

	do
	{
		std::this_thread::sleep_for(chr::seconds{ 1 });

		{
			LOCK(players.GetMutex());

			uint16_t currentSeconds{ currentSecondsLambda() };

			std::ranges::for_each(players.GetRef(), [currentSeconds, maxSeconds, &playersGuessedCount, &drawingPlayerSeconds](Player& player) {
				if (player.GetGuessStatus() && player.GetCurrentScore() == INT_MIN)
				{
					player.CalculateScore(currentSeconds, maxSeconds);
					std::cout << player.GetName() << " new current scores: " << player.GetCurrentScore() << ' ' << player.GetScore() << std::endl;
					playersGuessedCount++;
					drawingPlayerSeconds += currentSeconds;
				}
				});

			if (playersGuessedCount == players.GetRef().size())
			{
				std::cout << "All players guessed the word" << std::endl;
				break;
			}
		}

	} while (!m_stopped && currentTimeLambda() < drawingTime);

	{
		std::cout << "Calculating scores" << std::endl;

		LOCK(players.GetMutex());

		auto& drawingPlayer{ *std::ranges::find_if(players.GetRef(), [](const Player& player) {
				return player.GetRole() == common::game::PlayerRole::DRAWING;
			}) };

		drawingPlayer.CalculateScore(drawingPlayerSeconds, maxSeconds, playersGuessedCount - 1);
		drawingPlayer.AddScore();

		std::cout << "Drawing player scores:\n";
		std::cout << drawingPlayer.GetName() << " score: " << drawingPlayer.GetCurrentScore() << ' ' << drawingPlayer.GetScore() << std::endl;

		std::cout << "Guessing players scores:\n";
		std::ranges::for_each(players.GetRef(), [maxSeconds](Player& player) {

			if (!player.GetGuessStatus())
				player.CalculateScore(UINT16_MAX, maxSeconds);

			if (player.GetRole() == common::game::PlayerRole::GUESSING)
			{
				player.AddScore();
				std::cout << player.GetName() << " score: " << player.GetCurrentScore() << ' ' << player.GetScore() << std::endl;
			}
			});
	}

	std::cout << "Turn over" << std::endl;
}
