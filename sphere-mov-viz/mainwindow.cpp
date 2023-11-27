#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unistd.h>
#include <QLayout>
#include <QtMath>
#include <cmath>

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
	connect(timerSleep1, &QTimer::timeout, this, &MainWindow::restore);

	timerSleep2 = new QTimer();
	timerSleep2->setSingleShot(true);
	connect(timerSleep2, &QTimer::timeout, this, &MainWindow::restore2);

	timerSleep3 = new QTimer();
	timerSleep3->setSingleShot(true);
	connect(timerSleep3, &QTimer::timeout, this, &MainWindow::restore3);

	timerSleep4 = new QTimer();
	timerSleep4->setSingleShot(true);
	connect(timerSleep4, &QTimer::timeout, this, &MainWindow::restore4);

	connect(timer, &QTimer::timeout, this, &MainWindow::generateSignal);
	connect(this, &MainWindow::timerSignal, this, &MainWindow::handleTimerSignal);
	timer->start(1000 / FPS);

	m.setToIdentity();
	m2.setToIdentity();
	m3.setToIdentity();
	m4.setToIdentity();
	spm.setToIdentity();

	QPixmap p1("/home/nastya/cg-course-work/textures/green.jpg");
	QPixmap p2("/home/nastya/cg-course-work/textures/pink2.jpg");
	ui->label_7->setPixmap(p1);
	ui->label_8->setPixmap(p2);

	QPalette pal = ui->label_10->palette();
	pal.setColor(QPalette::Window, INIT_FONE);
	ui->label_10->setAutoFillBackground(true);
	ui->label_10->setPalette(pal);
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

void MainWindow::sphereMovement(QMatrix4x4 &m, float speed)
{
	m.rotate(QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, speed));
}

void MainWindow::swingsMovement(QMatrix4x4 &m, float degree, float tansposition, float speed, float *p, BaseObject *obj, bool *flag)
{
	if (std::fpclassify(speed) == FP_ZERO)
		return;
	if ((*p) > 0 )	{
		swingMovement(m, degree, tansposition, speed);
		obj->setGlobalTransform(m);
		(*p) += -speed;
	}
	else if ((*p) < 0) {
		swingMovement(m, degree, tansposition, -speed);
		obj->setGlobalTransform(m);
		(*p) -= -speed;
	}

	std::cout << (*p) <<std::endl;

	if ((*p) > 60) {
		(*p) = -0.0001;
		(*flag) = true;
	}
	else if ((*p) < -60) {
		(*p) = 0.0001;
		(*flag) = true;
	}
}

void MainWindow::generateSignal()
{
	emit timerSignal();
}

void MainWindow::handleTimerSignal()
{
	sphereMovement(spm, sphereSpeed);
	oglw->getObject(0)->setGlobalTransform(spm);
	if (flag1) {
		swingSpeed1 = 0.0;
		flag1 = false;
		timerSleep1->start(sleep);
	}

	if (flag2) {
		swingSpeed2 = 0.0;
		flag2 = false;
		timerSleep2->start(sleep);
	}

	if (flag3) {
		swingSpeed3 = 0.0;
		flag3 = false;
		timerSleep3->start(sleep);
	}

	if (flag4) {
		swingSpeed4 = 0.0;
		flag4 = false;
		timerSleep4->start(sleep);
	}

	swingsMovement(m2, 22.5, RADIUS, swingSpeed1, &_p, oglw->getObject(1), &flag1);
	swingsMovement(m3, 67.5, RADIUS, swingSpeed2, &p2, oglw->getObject(2), &flag2);
	swingsMovement(m4, 112.5, RADIUS, swingSpeed3, &p3, oglw->getObject(3), &flag3);
	swingsMovement(m, 157.5, RADIUS, swingSpeed4, &p4, oglw->getObject(4), &flag4);

	oglw->update();
}

void MainWindow::restore()
{
	swingSpeed1 = _swingSpeed1;
}

void MainWindow::restore2()
{
	swingSpeed2 = _swingSpeed2;
}

void MainWindow::restore3()
{
	swingSpeed3 = _swingSpeed3;
}

void MainWindow::restore4()
{
	swingSpeed4 = _swingSpeed4;
}

void MainWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);
	initState();
}

void MainWindow::initState()
{
	spm.setToIdentity();
	m.setToIdentity();
	m2.setToIdentity();
	m3.setToIdentity();
	m4.setToIdentity();

	swingMovement(m, 157.5, RADIUS, -15.0);
	oglw->getObject(4)->setGlobalTransform(m);

	swingMovement(m4, 112.5, RADIUS, 0.0);
	oglw->getObject(3)->setGlobalTransform(m4);

	swingMovement(m3, 67.5, RADIUS, 15.0);
	oglw->getObject(2)->setGlobalTransform(m3);

	swingMovement(m2, 22.5, RADIUS, 30.0);
	oglw->getObject(1)->setGlobalTransform(m2);

	oglw->getObject(0)->resetTransformations();
	oglw->getObject(0)->translate(QVector3D(0.0, SPHERE_Y, 0.0));

	oglw->update();
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
	timer->stop();
	sphereSpeed = arg1 / FPS;
	sleep = 42 * 1000 / arg1;
	_swingSpeed1 = -60 / (138 / sphereSpeed);
	_swingSpeed2 = -60 / (138 / sphereSpeed);
	_swingSpeed3 = -60 / (138 / sphereSpeed);
	_swingSpeed4 = -60 / (138 / sphereSpeed);
	swingSpeed1 = -60 / (138 / sphereSpeed);
	swingSpeed2 = -60 / (138 / sphereSpeed);
	swingSpeed3 = -60 / (138 / sphereSpeed);
	swingSpeed4 = -60 / (138 / sphereSpeed);
	_p = 0.000001;
	p2 = -44.000001;
	p3 = -29.000001;
	p4 = -14.000001;
	flag1 = true;
	flag2 = false;
	flag3 = false;
	flag4 = false;
	initState();
	timer->start(1000 / FPS);
}


void MainWindow::on_pushButton_clicked()
{
	QFile sphereTexturePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстуру", "/home/nastya/cg-course-work/textures");
}

