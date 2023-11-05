//#include "mainwindow.h"
#include "glwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GLWidget oglw;
	oglw.show();

//	return a.exec();
	return a.exec();
}
