#include "Image.h"

Color::Color(uint32_t rgbHex) :
	r{ (rgbHex >> 16) & 0xFF },
	g{ (rgbHex >> 8) & 0xFF },
	b{ rgbHex & 0xFF }
{ /* empty */ }

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
	for (int i = m_updates.size() - 1 && m_updates[i].timestamp >= timestamp; i >= 0; i--)
		updates.push_back(m_updates[i]);
	
	return updates;
}
