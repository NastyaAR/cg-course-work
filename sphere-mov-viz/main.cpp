#include "mainwindow.h"
#include "glwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

//	QSurfaceFormat format;
//	format.setSamples(16);

//	QSurfaceFormat::setDefaultFormat(format);
//	GLWidget oglw;

//	oglw.show();

	MainWindow w;
	w.show();
	return a.exec();
}
