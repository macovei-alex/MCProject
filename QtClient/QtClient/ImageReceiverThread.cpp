#include "ImageReceiverThread.h"

#include <QScreen>
#include <QDebug>
#include <QWidget>

#include "macro.h"
#include "services.h"

#ifdef ONLINE
ImageReceiverThread::ImageReceiverThread(uint64_t roomID, bool& keepGoing, QWidget* parent) :
	QThread{ parent },
	keepGoing{ keepGoing },
	roomID{ roomID }
{
	/* empty */
}

void ImageReceiverThread::run()
{
	using std::chrono_literals::operator""s;
	try
	{
		while (keepGoing)
		{
			auto commonPoints{ std::move(services::ReceiveImageUpdates(roomID)) };

			if (commonPoints.empty())
			{
				std::this_thread::sleep_for(0.25s);
				continue;
			}

			qDebug() << "Received points: " << commonPoints.size();

			QList<MyLine>* newLines{ new QList<MyLine> };
			MyLine* line;

			for (size_t i = 0; i < commonPoints.size(); i++)
			{
				line = new MyLine;
				line->drawState = (commonPoints[i].color == MyLine::ERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING);

				while (i < commonPoints.size() && commonPoints[i].color != MyLine::INVALID_COLOR_INT)
				{
					line->points.emplace_back(commonPoints[i].x, commonPoints[i].y);
					i++;
				}

				newLines->emplace_back(std::move(*line));
				i++;
			}

			qDebug() << "Received lines: " << newLines->size();

			emit LinesReceivedSignal(newLines);

			std::this_thread::sleep_for(0.25s);
		}
	}
	catch (const std::exception& e)
	{
		qDebug() << e.what() << '\n';
	}
}
#endif