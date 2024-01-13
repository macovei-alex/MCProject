#include "ImageThread.h"
#include "macro.h"

#ifdef ONLINE
#include <QDebug>
#include <QWidget>

#include "services.h"

ImageThread::ImageThread(uint64_t roomID, bool& keepGoing, QWidget* parent) :
	QThread{ parent },
	keepGoing{ keepGoing },
	roomID{ roomID }
{
	/* empty */
}

void ImageThread::run()
{
	while (keepGoing)
	{
		if (IsPaused())
		{
			QThread::msleep(500);
			continue;
		}

		try
		{
			while (!IsPaused() && keepGoing)
			{
				auto commonPoints{ std::move(services::ReceiveImageUpdates(roomID)) };

				if (commonPoints.empty())
				{
					QThread::msleep(500);
					continue;
				}

				QList<Line>* newLines{ new QList<Line> };
				Line* line;

				for (size_t i = 0; i < commonPoints.size(); i++)
				{
					line = new Line;
					line->drawState = (commonPoints[i].color == Line::kERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING);

					while (i < commonPoints.size() && commonPoints[i].color != Line::INVALID_COLOR_INT)
					{
						line->points.emplace_back(commonPoints[i].x, commonPoints[i].y);
						i++;
					}

					newLines->emplace_back(std::move(*line));
					i++;
				}

				emit ImageSignal(newLines);

				QThread::msleep(500);
			}
		}
		catch (const std::exception& e)
		{
			qDebug() << e.what() << '\n';
		}
	}
}
#endif