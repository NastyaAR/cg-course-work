#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unistd.h>
#include <QLayout>
#include <QtMath>
#include <cmath>
#include <chrono>

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

	this->setFocus();
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

	for (int i = 0; i < timers.size(); i++)
		timers[i]->stop();

	float arg1 = ui->spinBox->value();
	sphereSpeed = arg1 / FPS;
	sleep = 43 * 1000 / arg1;

	initSwingSpeed = -60 / (138 / sphereSpeed);
	for (int i = 0; i < OBJ_NUMBER - 1; i++)
		swingSpeeds[i] = -60 / (138 / sphereSpeed);

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

	drives[0] = 0.000001;
	drives[1] = -45.000001;
	drives[2] = -30.000001;
	drives[3] = -15.000001;
	flags[0] = true;
	flags[1] = false;
	flags[2] = false;
	flags[3] = false;

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
	initState();
	timer->start(1000 / FPS);
}


void MainWindow::on_pushButton_clicked()
{
	QString sphereTexturePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстуру", "/home/nastya/cg-course-work/textures");
	setLabel(ui->label_7, sphereTexturePath);
	timer->stop();
	oglw->getObject(0)->changeTexture(sphereTexturePath);
	initState();
	timer->start(1000 / FPS);
}


void MainWindow::on_pushButton_2_clicked()
{
	QString swingTexturePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстуру", "/home/nastya/cg-course-work/textures");
	setLabel(ui->label_8, swingTexturePath);
	timer->stop();
	for (int i = 1; i < OBJ_NUMBER; i++)
		oglw->getObject(i)->changeTexture(swingTexturePath);
	initState();
	timer->start(1000 / FPS);
}


void MainWindow::on_pushButton_3_clicked()
{
	QColor clr = QColorDialog::getColor(INIT_FONE, nullptr, "Выберите цвет фона");
	setLabel(ui->label_10, clr);
	timer->stop();
	oglw->setFone(clr);
	initState();
	timer->start(1000 / FPS);
}

void MainWindow::insertIntoTable(QVector3D direction, float power)
{
	std::ostringstream oss1;
	std::ostringstream oss2;
	oss1 << "(" << direction.x() << ", " << direction.y() << ", " << direction.z() << ")";
	oss2 << power;

	ui->tableWidget->insertRow(ui->tableWidget->rowCount());
	ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(oss1.str().data()));
	ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(oss2.str().data()));
}

std::tuple<QVector4D, float> MainWindow::formSearchLight(int i)
{
	QTableWidgetItem *dir = ui->tableWidget->item(i, 0);
	QTableWidgetItem *pow = ui->tableWidget->item(i, 1);
	QString dirStr = dir->text();
	dirStr = dirStr.mid(1);
	dirStr.chop(1);
	QStringList strs = dirStr.split(",");
	QVector3D direction(strs[0].toFloat(), strs[1].toFloat(), strs[2].toFloat());
	float power = pow->text().toFloat();

	return std::tuple(QVector4D(direction, 0.0).normalized(), power);
}

double MainWindow::measureTime()
{
	auto start = std::chrono::steady_clock::now();
	double sum = 0;
	for (int i = 0; i < N_REPS; i++) {
		auto start = std::chrono::steady_clock::now();
		oglw->update();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> duration = end - start;
		sum += duration.count();
	}

	return sum/N_REPS;
}

void MainWindow::on_pushButton_4_clicked()
{
	MyDialog dlg;
	float values[4];
	bool ok;

	timer->stop();
	initState();
	auto dlgAns = dlg.exec();
	if (dlgAns == QDialog::Accepted) {
		for (int i = 0; i < 4; i++) {
			std::tie(values[i], ok) = dlg.getValue(i);
			if (!ok)
				return;
		}
		QVector3D direction(values[0], values[1], values[2]);
		insertIntoTable(direction, values[3]);
		oglw->addLight(direction, values[3]);
		timer->start(1000 / FPS);
	}
	if (dlgAns == QDialog::Rejected) {
		timer->start(1000 / FPS);
	}
}


void MainWindow::on_pushButton_5_clicked()
{
	int delRow = ui->tableWidget->currentRow();
	if (ui->tableWidget->rowCount() == 1) {
		return;
	}
	timer->stop();
	initState();
	QVector4D direction;
	float power;
	std::tie(direction, power) = formSearchLight(delRow);
	oglw->delLight(direction, power);
	ui->tableWidget->removeRow(delRow);
	timer->start(1000 / FPS);
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
	oglw->getObject(0)->changeMaterial(1, arg1);
}


void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
	oglw->getObject(0)->changeMaterial(0, arg1);
}


void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
	oglw->getObject(0)->changeMaterial(2, arg1);
}


void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
	for (int i = 1; i < OBJ_NUMBER; i++)
		oglw->getObject(i)->changeMaterial(1, arg1);
}


void MainWindow::on_doubleSpinBox_6_valueChanged(double arg1)
{
	for (int i = 1; i < OBJ_NUMBER; i++)
		oglw->getObject(i)->changeMaterial(0, arg1);
}


void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
	for (int i = 1; i < OBJ_NUMBER; i++)
		oglw->getObject(i)->changeMaterial(2, arg1);
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Home) {
		QVector<double> seconds;
		timer->stop();
		initState();
		auto l = oglw->getLights();
		auto c = oglw->getCurLights();
		QVector<Light *> newLights;
		int cur_lights = 0;
		for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
			newLights.append(new Light(DIRECTIONAL));
			newLights[i]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
			newLights[i]->Power = 0.9;
			newLights[i]->position = QVector4D(0.0f, 0.0f, 10.0f, 1.0f);
			newLights[i]->direction = QVector4D(0.0f, -1.0f, 0.0f, 0.0f);
			newLights[i]->used = false;
		}
		for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
			newLights[i]->used = true;
			cur_lights++;
			oglw->setLights(newLights, cur_lights);
			seconds.push_front(measureTime());
		}
		writeTime(seconds);
		oglw->setLights(l, c);
		timer->start(1000 / FPS);
	}
}

void MainWindow::writeTime(QVector<double> seconds)
{
	std::ofstream outputFile;
	outputFile.open("/home/nastya/cg-course-work/time.txt", std::ios::out | std::ios::app);

	if (outputFile.is_open()) {
		std::ostringstream oss;
		std::copy(seconds.begin(), seconds.end() - 1, std::ostream_iterator<double>(oss, ", "));
		std::string result = oss.str();
		outputFile << result;
		outputFile.close();
	}
}


