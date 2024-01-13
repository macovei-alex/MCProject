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
	using std::chrono_literals::operator""s;
	while (keepGoing)
	{
		if (IsPaused())
		{
			std::this_thread::sleep_for(0.25s);
			continue;
		}

		try
		{
			while (!IsPaused() && keepGoing)
			{
				auto commonPoints{ std::move(services::ReceiveImageUpdates(roomID)) };

				if (commonPoints.empty())
				{
					std::this_thread::sleep_for(0.25s);
					continue;
				}

				qDebug() << "Received points: " << commonPoints.size();

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

				qDebug() << "Received lines: " << newLines->size();

				emit ImageSignal(newLines);

				std::this_thread::sleep_for(0.25s);
			}
		}
		catch (const std::exception& e)
		{
			qDebug() << e.what() << '\n';
		}
	}
}
#endif