#include "Line.h"

const int32_t Line::kDRAWING_COLOR_INT{ 0x000000 };
const int32_t Line::kERASING_COLOR_INT{ 0xFFFFFF };

#ifdef ONLINE
const int32_t Line::INVALID_COLOR_INT{ 0xFFFFFE };

Line::Line(std::vector<common::img::Point>&& commonPoints, uint32_t color) :
	drawState{ color == kERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING },
	points{ static_cast<qsizetype>(commonPoints.size()), QPoint() }
{
	size_t pos = 0;
	for (const auto& commonPoint : commonPoints)
		points[pos++] = QPoint{ commonPoint.x, commonPoint.y };
}

std::vector<common::img::Point> Line::ToCommonPoints() const
{
	std::vector<common::img::Point> commonPoints{ static_cast<size_t>(points.size()), common::img::Point{} };
	size_t pos = 0;

	for (const auto& point : points)
	{
		commonPoints[pos++] = {
			point.x(),
			point.y(),
			(drawState == DrawingState::DRAWING ? kDRAWING_COLOR_INT : kERASING_COLOR_INT) };
	}

	return commonPoints;
}
#endif