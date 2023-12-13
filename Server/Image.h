#pragma once

#include "..\Common\commonUtils.h"

#include <vector>
#include <crow.h>

class Image
{
public:
	Image() = default;
	Image(const Image& other) = default;
	Image& operator=(const Image& other) = default;
	Image(Image&& other) = default;
	Image& operator=(Image&& other) = default;

public:
	void AddUpdate(const utils::img::Update& update);
	void AddUpdate(utils::img::Update&& update);
	std::vector<utils::img::Update> GetUpdatesAfter(uint64_t timestamp);
	crow::json::wvalue GetUpdatesJsonAfter(uint64_t timestamp);
	
public:
	static crow::json::wvalue UpdatesToJson(const std::vector<utils::img::Update>& updates);

private:
	std::vector<utils::img::Update> m_updates;
};

