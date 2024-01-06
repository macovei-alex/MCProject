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

struct MyLine
{
	static const int32_t DRAWING_COLOR_INT;
	static const int32_t ERASING_COLOR_INT;

#ifdef ONLINE
	static const int32_t INVALID_COLOR_INT;
#endif

	QList<QPoint> points;
	DrawingState drawState;

	MyLine() = default;

#ifdef ONLINE
	MyLine(std::vector<common::img::Point>&& points, uint32_t color);
	std::vector<common::img::Point> ToCommonPoints() const;
#endif

};
