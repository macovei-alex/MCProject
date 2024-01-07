#pragma once

#include "macro.h"

#include <cstdint>
#include <QPoint>
#include <QList>

#ifdef ONLINE
#include <vector>
#include "common.h"
#endif

#include "DrawingState.h"

struct Line
{
	static const int32_t kDRAWING_COLOR_INT;
	static const int32_t kERASING_COLOR_INT;

#ifdef ONLINE
	static const int32_t INVALID_COLOR_INT;
#endif

	QList<QPoint> points;
	DrawingState drawState;

	Line() = default;

#ifdef ONLINE
	Line(std::vector<common::img::Point>&& points, uint32_t color);
	std::vector<common::img::Point> ToCommonPoints() const;
#endif

};
