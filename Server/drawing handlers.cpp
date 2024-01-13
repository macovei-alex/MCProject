#include "Server.h"

Server& Server::DrawingHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::draw::updatesParam).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		static const crow::json::wvalue errorValue{ crow::json::wvalue::list{{literals::error, literals::emptyCString}} };

		uint64_t timestamp;

		try
		{
			if (char* timestampChar{ request.url_params.get(literals::jsonKeys::draw::timestamp) };
				timestampChar != nullptr)
				timestamp = std::stoull(timestampChar);
			else
				throw std::exception{ "Timestamp key not found" };
		}
		catch (const std::exception& ex)
		{
			Log(ex.what(), Logger::Level::Error);
			return errorValue;
		}

		auto updates{ std::move(m_games[gameID].GetImage().GetUpdatesJsonAfter(timestamp)) };
		return updates;
			});


	CROW_ROUTE(m_app, literals::routes::game::draw::updatesParam).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		const static std::string pointXStrKey{ literals::jsonKeys::draw::pointX };
		const static std::string pointYStrKey{ literals::jsonKeys::draw::pointY };
		const static std::string colorStrKey{ literals::jsonKeys::draw::color };

		if (request.body.empty())
			return crow::response{ 404, "empty request body" };

		auto gameIt{ m_games.find(gameID) };
		if (gameIt == m_games.end())
			return crow::response{ 404, std::format("Invalid game ID < {} >", gameID) };

		auto jsonMap{ utils::ParseRequestBody(utils::DecodeMessage(request.body)) };
		auto jsonVector{ utils::ListOfMapsFromJsonListStr(jsonMap[literals::jsonKeys::draw::points]) };

		/*for (const auto& jsonPoint : jsonVector)
		{
			for (const auto& [key, value] : jsonPoint)
				if (std::holds_alternative<int64_t>(value))
					std::cout << key << " : " << std::get<int64_t>(value) << '\n';
				else
					std::cout << key << " : " << std::get<std::string>(value) << '\n';
			std::cout << "\n\n";
		}*/

		Image& image{ gameIt->second.GetImage() };

		image.GetMutex().lock();
		for (const auto& pointMap : jsonVector)
		{
			try
			{
				common::img::Point point{
					std::get<int64_t>(pointMap.at(pointXStrKey)),
					std::get<int64_t>(pointMap.at(pointYStrKey)),
					std::get<int64_t>(pointMap.at(colorStrKey))
				};

				image.AddUpdate(common::img::Update{ point, utils::MillisFromDateTime(std::chrono::system_clock::now()) });
			}
			catch (const std::exception& exception)
			{
				Log(exception.what(), Logger::Level::Error);
				return crow::response{ 404, exception.what() };
			}
		}
		image.GetMutex().unlock();

		Log("new updates added", Logger::Level::Info);
		return crow::response{ 200 };
			});

	Log("Draw handlers set");
	return *this;
}