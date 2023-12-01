#include "mainwindow.h"
#include <windows.h>

#include <QApplication>

int main(int argc, char* argv[])
{
	// Find out the current directory
	TCHAR buffer[256];
	DWORD buffer_size = MAX_PATH;
	DWORD length = GetCurrentDirectory(buffer_size, buffer);
	std::wstring wstr{ buffer, length };
	qDebug() << "*****FOLDER***** : " << QString::fromStdWString(wstr);


	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
