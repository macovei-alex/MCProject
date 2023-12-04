#include "Image.h"

#include "..\Common\constantLiterals.h"

Color::Color(uint32_t rgbHex) :
	r{ (rgbHex >> 16) & 0xFF },
	g{ (rgbHex >> 8) & 0xFF },
	b{ rgbHex & 0xFF }
{
	/* empty */
}

Color::Color(int32_t rgbHex) :
	r{ uint8_t((rgbHex >> 16) & 0xFF) },
	g{ uint8_t((rgbHex >> 8) & 0xFF) },
	b{ uint8_t(rgbHex & 0xFF) }
{
	/* empty */
}

void Image::AddUpdate(const Update& update)
{
	this->m_updates.push_back(update);
}

void Image::AddUpdate(Update&& update)
{
	this->m_updates.push_back(std::move(update));
}

std::vector<Update> Image::GetUpdatesAfter(uint64_t timestamp)
{
	std::vector<Update> updates;
	for (auto it{ m_updates.rbegin() }; it != m_updates.rend() && it->timestamp >= timestamp; it++)
		updates.push_back(*it);

	return updates;
}

crow::json::wvalue Image::GetUpdatesJsonAfter(uint64_t timestamp)
{
	return UpdatesToJson(GetUpdatesAfter(timestamp));
}

crow::json::wvalue Image::UpdatesToJson(const std::vector<Update>& updates)
{
	crow::json::wvalue::list updatesJsonList{};
	updatesJsonList.reserve(updates.size());

	for (const auto& update : updates)
	{
		int64_t rgb{
			(update.point.color.r << 16)
			| (update.point.color.g << 8)
			| update.point.color.b };

		crow::json::wvalue updateJson{
			{literals::jsonKeys::draw::pointX, std::to_string(update.point.x) },
			{literals::jsonKeys::draw::pointY, std::to_string(update.point.y) },
			{literals::jsonKeys::draw::color, std::to_string(rgb) }
		};

		updatesJsonList.push_back(std::move(updateJson));
	}

	return crow::json::wvalue{ updatesJsonList };
}
