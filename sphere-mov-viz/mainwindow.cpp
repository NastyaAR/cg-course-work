#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unistd.h>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QSurfaceFormat format;
	format.setSamples(16);

	QSurfaceFormat::setDefaultFormat(format);

	oglw = new GLWidget();
	ui->gridLayout->addWidget(oglw);
	oglw->obj5->resetTransformations();

	timer = new QTimer(this);
	int frequency = 5; // Частота генерации в секундах
	connect(timer, &QTimer::timeout, this, &MainWindow::generateSignal);
	timer->start(1000 / frequency);

	connect(this, &MainWindow::timerSignal, this, &MainWindow::handleTimerSignal);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete oglw;
	delete timer;
}


void MainWindow::on_pushButton_clicked()
{
	timer->stop();
}

void MainWindow::generateSignal()
{
	emit timerSignal();
}

void MainWindow::handleTimerSignal()
{
//	if (init.x() == -6.0 && init.z() == -6.0) {
//		oglw->obj5->translate(QVector3D(0.1, 0.0, -0.1));
//		init.setX(init.x() + 0.1);
//		init.setZ(init.z() - 0.1);
//	}
//	else {
//		oglw->obj5->translate(QVector3D(-0.1, 0.0, 0.1));
//		init.setX(init.x() - 0.1);
//		init.setZ(init.z() + 0.1);
//	}
	oglw->obj5->translate(QVector3D(0.0, 100.0, 0.0));
	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 45.5));
	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -3.0));
	oglw->obj5->translate(QVector3D(0.0, -100.0, 0.0));
	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -45.5));
//	oglw->obj5->translate(QVector3D(0.0, -100.0, 0.0));
	oglw->update();
}


void MainWindow::on_pushButton_2_clicked()
{
	if (init.x() == -6 && init.z() == -6) {
		oglw->obj5->translate(QVector3D(0.1, 0.0, -0.1));
		init.setX(init.x() + 0.1);
		init.setZ(init.z() - 0.1);
	}
	else {
		oglw->obj5->translate(QVector3D(-0.1, 0.0, 0.1));
		init.setX(init.x() - 0.1);
		init.setZ(init.z() + 0.1);
	}
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 22.5));
//	oglw->obj5->translate(QVector3D(100.0, 0.0, 0.0));
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, 10.0));
//	oglw->obj5->translate(QVector3D(-100.0, 0.0, 0.0));
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -22.5));
	oglw->update();
}

