#include "MyLine.h"

const int32_t MyLine::DRAWING_COLOR_INT{ 0x000000 };
const int32_t MyLine::ERASING_COLOR_INT{ 0xFFFFFF };

#ifdef ONLINE
const int32_t MyLine::INVALID_COLOR_INT{ 0xFFFFFE };

MyLine::MyLine(std::vector<common::img::Point>&& commonPoints, uint32_t color) :
	drawState{ color == ERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING },
	points{ static_cast<qsizetype>(commonPoints.size()), QPoint() }
{
	size_t pos = 0;
	for (const auto& commonPoint : commonPoints)
		points[pos++] = QPoint{ commonPoint.x, commonPoint.y };
}

std::vector<common::img::Point> MyLine::ToCommonPoints() const
{
	std::vector<common::img::Point> commonPoints{ static_cast<size_t>(points.size()), common::img::Point{} };
	size_t pos = 0;

	for (const auto& point : points)
	{
		commonPoints[pos++] = {
			point.x(),
			point.y(),
			(drawState == DrawingState::DRAWING ? DRAWING_COLOR_INT : ERASING_COLOR_INT) };
	}

	return commonPoints;
}
#endif