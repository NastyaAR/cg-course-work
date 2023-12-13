#include "mainwindow.h"
#include "glwidget.h"
#include <QApplication>

#include <QtTest>

#include "testcamera.h"
#include "testlight.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	QTest::qExec(new TestCamera, argc, argv);
	QTest::qExec(new TestLight, argc, argv);
	return a.exec();
}
