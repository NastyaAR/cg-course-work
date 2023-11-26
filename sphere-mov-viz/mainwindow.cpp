#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unistd.h>
#include <QLayout>
#include <QtMath>

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

	timer = new QTimer();
	timerSleep1 = new QTimer();
	timerSleep1->setSingleShot(true);
	connect(timerSleep1, &QTimer::timeout, this, &MainWindow::restore);  // Подключаем timeout таймера к слоту восстановления переменной

	timerSleep2 = new QTimer();
	timerSleep2->setSingleShot(true);
	connect(timerSleep2, &QTimer::timeout, this, &MainWindow::restore2);

	timerSleep3 = new QTimer();
	timerSleep3->setSingleShot(true);
	connect(timerSleep3, &QTimer::timeout, this, &MainWindow::restore3);

	timerSleep4 = new QTimer();
	timerSleep4->setSingleShot(true);
	connect(timerSleep4, &QTimer::timeout, this, &MainWindow::restore4);

	int frequency = 30; // Частота генерации в секундах
	connect(timer, &QTimer::timeout, this, &MainWindow::generateSignal);
	connect(this, &MainWindow::timerSignal, this, &MainWindow::handleTimerSignal);
	timer->start(1000 / frequency);

	m.setToIdentity();
	m2.setToIdentity();
	m3.setToIdentity();
	m4.setToIdentity();
}

MainWindow::~MainWindow()
{
	delete timer;
	delete timerSleep1;
	delete timerSleep2;
	delete timerSleep3;
	delete timerSleep4;
	delete ui;
	delete oglw;
}

void MainWindow::swingMovement(QMatrix4x4 &m, float degree, float tansposition, float speed)
{
	m.rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, degree));
	m.translate(QVector3D(0.0, tansposition, 0.0));
	m.rotate(QQuaternion::fromAxisAndAngle(0.0, 0.0, 1.0, speed));
	m.translate(QVector3D(0.0, -tansposition, 0.0));
	m.rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -degree));
}

void MainWindow::swingsMovement(QMatrix4x4 &m, float degree, float tansposition, float speed, float *p, BaseObject *obj, bool *flag)
{
	if (speed > -0.5 && speed < 0.5)
		return;
	if ((*p) > 0 )	{
		swingMovement(m, degree, tansposition, speed);
		obj->setGlobalTransform(m);
		(*p) += 1;
	}
	else if ((*p) < 0) {
		swingMovement(m, degree, tansposition, -speed);
		obj->setGlobalTransform(m);
		(*p) -= 1;
	}

	if ((*p) > 60) {
		(*p) = -0.0001;
		(*flag) = true;
	}
	else if ((*p) < -60) {
		(*p) = 0.0001;
		(*flag) = true;
	}
}


void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::generateSignal()
{
	emit timerSignal();
}

void MainWindow::handleTimerSignal()
{
	oglw->obj1->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 2.48));
	if (flag1) {
		sp = 0.0;
		flag1 = false;
		timerSleep1->start(370);
	}

	if (flag2) {
		sp2 = 0.0;
		flag2 = false;
		timerSleep2->start(370);
	}

	if (flag3) {
		sp3 = 0.0;
		flag3 = false;
		timerSleep3->start(370);
	}

	if (flag4) {
		sp4 = 0.0;
		flag4 = false;
		timerSleep4->start(370);
	}

	swingsMovement(m2, 22.5, RADIUS, sp, &_p, oglw->obj2, &flag1);
	swingsMovement(m3, 67.5, RADIUS, sp2, &p2, oglw->obj3, &flag2);
	swingsMovement(m4, 112.5, RADIUS, sp3, &p3, oglw->obj4, &flag3);
	swingsMovement(m, 157.5, RADIUS, sp4, &p4, oglw->obj5, &flag4);

	oglw->update();
}

void MainWindow::restore()
{
	sp = -1.0;
}

void MainWindow::restore2()
{
	sp2 = -1.0;
}

void MainWindow::restore3()
{
	sp3 = -1.0;
}

void MainWindow::restore4()
{
	sp4 = -1.0;
}

void MainWindow::on_pushButton_2_clicked()
{
//	int L = 5;
//	float r1 = qDegreesToRadians(vert);
//	float r2 = qDegreesToRadians(hor);
//	float dx = L * qSin(r1) * qCos(r2);
//	float dy = L * qSin(r1) * qSin(r2);
//	float dz = L * qCos(r1);
//	oglw->obj5->translate(QVector3D(dx, dy, dz));
//	vert -= 1;
//	hor += 1;
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, 22.5));
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -3.0));
//	oglw->obj5->translate(QVector3D(0.0, -100.0, 0.0));
//	oglw->obj5->rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, -22.5));
//	oglw->obj5->translate(QVector3D(0.0, -100.0, 0.0));
	oglw->update();
}

void MainWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);

	swingMovement(m, 157.5, RADIUS, -15.0);
	oglw->obj5->setGlobalTransform(m);

	swingMovement(m4, 112.5, RADIUS, 0.0);
	oglw->obj4->setGlobalTransform(m4);

	swingMovement(m3, 67.5, RADIUS, 15.0);
	oglw->obj3->setGlobalTransform(m3);

	swingMovement(m2, 22.5, RADIUS, 30.0);
	oglw->obj2->setGlobalTransform(m2);

	oglw->update();
}

