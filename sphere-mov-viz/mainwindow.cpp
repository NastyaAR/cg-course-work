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

	initTimers();

	timer = new QTimer();
	connect(timer, &QTimer::timeout, this, &MainWindow::generateSignal);
	connect(this, &MainWindow::timerSignal, this, &MainWindow::handleTimerSignal);
	timer->start(1000 / FPS);

	for (int i = 0; i < OBJ_NUMBER; i++) {
		matrixes.push_back(QMatrix4x4());
		matrixes[i].setToIdentity();
	}

	setLabel(ui->label_7, QString("/home/nastya/cg-course-work/textures/green.jpg"));
	setLabel(ui->label_8, QString("/home/nastya/cg-course-work/textures/pink2.jpg"));
	setLabel(ui->label_10, INIT_FONE);
}

MainWindow::~MainWindow()
{
	delete timer;
	for (int i = 0; i < OBJ_NUMBER; i++)
		delete timers[i];
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
	sphereMovement(matrixes[0], sphereSpeed);
	oglw->getObject(0)->setGlobalTransform(matrixes[0]);
	if (flags[0]) {
		swingSpeeds[0] = 0.0;
		flags[0] = false;
		timers[0]->start(sleep);
	}

	if (flags[1]) {
		swingSpeeds[1] = 0.0;
		flags[1] = false;
		timers[1]->start(sleep);
	}

	if (flags[2]) {
		swingSpeeds[2] = 0.0;
		flags[2] = false;
		timers[2]->start(sleep);
	}

	if (flags[3]) {
		swingSpeeds[3] = 0.0;
		flags[3] = false;
		timers[3]->start(sleep);
	}

	swingsMovement(matrixes[1], 22.5, RADIUS, swingSpeeds[0], &drives[0], oglw->getObject(1), &flags[0]);
	swingsMovement(matrixes[2], 67.5, RADIUS, swingSpeeds[1], &drives[1], oglw->getObject(2), &flags[1]);
	swingsMovement(matrixes[3], 112.5, RADIUS, swingSpeeds[2], &drives[2], oglw->getObject(3), &flags[2]);
	swingsMovement(matrixes[4], 157.5, RADIUS, swingSpeeds[3], &drives[3], oglw->getObject(4), &flags[3]);

	oglw->update();
}

void MainWindow::restore()
{
	swingSpeeds[0] = initSwingSpeed;
}

void MainWindow::restore2()
{
	swingSpeeds[1] = initSwingSpeed;
}

void MainWindow::restore3()
{
	swingSpeeds[2] = initSwingSpeed;
}

void MainWindow::restore4()
{
	swingSpeeds[3] = initSwingSpeed;
}

void MainWindow::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);
	initState();
}

void MainWindow::initState()
{
	for (int i = 0; i < OBJ_NUMBER; i++)
		matrixes[i].setToIdentity();

	swingMovement(matrixes[4], 157.5, RADIUS, -15.0);
	oglw->getObject(4)->setGlobalTransform(matrixes[4]);

	swingMovement(matrixes[3], 112.5, RADIUS, 0.0);
	oglw->getObject(3)->setGlobalTransform(matrixes[3]);

	swingMovement(matrixes[2], 67.5, RADIUS, 15.0);
	oglw->getObject(2)->setGlobalTransform(matrixes[2]);

	swingMovement(matrixes[1], 22.5, RADIUS, 30.0);
	oglw->getObject(1)->setGlobalTransform(matrixes[1]);

	oglw->getObject(0)->resetTransformations();
	oglw->getObject(0)->translate(QVector3D(0.0, SPHERE_Y, 0.0));

	oglw->update();
}

void MainWindow::setLabel(QLabel *lbl, QString path)
{
	QPixmap pxm(path);
	lbl->setPixmap(pxm);
}

void MainWindow::setLabel(QLabel *lbl, QColor clr)
{
	QPalette pal = lbl->palette();
	pal.setColor(QPalette::Window, clr);
	lbl->setAutoFillBackground(true);
	lbl->setPalette(pal);
}

void MainWindow::initTimers()
{
	for (int i = 0; i < OBJ_NUMBER; i++) {
		timers.push_back(new QTimer());
		timers[i]->setSingleShot(true);
	}

	connect(timers[0], &QTimer::timeout, this, &MainWindow::restore);
	connect(timers[1], &QTimer::timeout, this, &MainWindow::restore2);
	connect(timers[2], &QTimer::timeout, this, &MainWindow::restore3);
	connect(timers[3], &QTimer::timeout, this, &MainWindow::restore4);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
	timer->stop();
	sphereSpeed = arg1 / FPS;
	sleep = 43 * 1000 / arg1;

	initSwingSpeed = -60 / (138 / sphereSpeed);
	for (int i = 0; i < OBJ_NUMBER - 1; i++)
		swingSpeeds[i] = -60 / (138 / sphereSpeed);

	drives[0] = 0.000001;
	drives[1] = -44.000001;
	drives[2] = -29.000001;
	drives[3] = -14.000001;
	flags[0] = true;
	flags[1] = false;
	flags[2] = false;
	flags[3] = false;
	initState();
	timer->start(1000 / FPS);
}


void MainWindow::on_pushButton_clicked()
{
	QString sphereTexturePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстуру", "/home/nastya/cg-course-work/textures");
	setLabel(ui->label_7, sphereTexturePath);
}


void MainWindow::on_pushButton_2_clicked()
{
	QString swingTexturePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстуру", "/home/nastya/cg-course-work/textures");
	setLabel(ui->label_8, swingTexturePath);
}


void MainWindow::on_pushButton_3_clicked()
{
	QColor clr = QColorDialog::getColor(INIT_FONE, nullptr, "Выберите цвет фона");
	setLabel(ui->label_10, clr);
}


void MainWindow::on_pushButton_4_clicked()
{
	MyDialog dlg;
	dlg.exec();
}

