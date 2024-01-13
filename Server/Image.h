#pragma once

#include <vector>
#include <crow.h>

#include "common.h"

class Image
{
public:
	Image() noexcept;
	Image(const Image& other) = default;
	Image& operator=(const Image& other) = default;
	Image(Image&& other) = default;
	Image& operator=(Image&& other) = default;

public:
	void AddUpdate(const common::img::Update& update);
	void AddUpdate(common::img::Update&& update);
	void Clear();
	std::vector<common::img::Update> GetUpdatesAfter(uint64_t timestamp);
	crow::json::wvalue GetUpdatesJsonAfter(uint64_t timestamp);
	std::mutex& GetMutex();
	
public:
	static crow::json::wvalue UpdatesToJson(const std::vector<common::img::Update>& updates);

private:
	std::vector<common::img::Update> m_updates;
	std::shared_ptr<std::mutex> m_sharedMutex;
};

