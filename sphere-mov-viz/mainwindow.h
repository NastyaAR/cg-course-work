#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"
#include <iostream>
#include <QShowEvent>
#include <QColor>
#include <QFileDialog>
#include <QColorDialog>
#include <fstream>

#include "mydialog.h"

#define RADIUS 5.1
#define FPS 30
#define INIT_FONE QColor(255, 216.75, 186.15)
#define N_REPS 10000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void swingMovement(QMatrix4x4 &m, float degree, float tansposition, float speed);
	void swingsMovement(QMatrix4x4 &m, float degree, float tansposition, float speed, float *p,
						BaseObject *obj, bool *flag);
	void sphereMovement(QMatrix4x4 &m, float speed);
	void showEvent(QShowEvent *event);
	void initState();
	void setLabel(QLabel *lbl, QString path);
	void setLabel(QLabel *lbl, QColor clr);
	void initTimers();
	void insertIntoTable(QVector3D direction, float power);
	std::tuple<QVector4D, float> formSearchLight(int i);
	double measureTime();
	void writeTime(QVector<double> seconds);

private slots:
	void generateSignal();
	void handleTimerSignal();
	void restore();
	void restore2();
	void restore3();
	void restore4();

	void on_spinBox_valueChanged(int arg1);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();
	void on_pushButton_5_clicked();
	void keyPressEvent(QKeyEvent *event);

	void on_doubleSpinBox_valueChanged(double arg1);

	void on_doubleSpinBox_2_valueChanged(double arg1);

	void on_doubleSpinBox_3_valueChanged(double arg1);

	void on_doubleSpinBox_5_valueChanged(double arg1);

	void on_doubleSpinBox_6_valueChanged(double arg1);

	void on_doubleSpinBox_4_valueChanged(double arg1);

signals:
	void timerSignal();
	void initSignal();

private:
	Ui::MainWindow *ui;
	GLWidget *oglw;
	QTimer *timer;
	QVector<QTimer *> timers;
	QVector<QMatrix4x4> matrixes;
	float drives[OBJ_NUMBER - 1] = {0.000001, -45.000001,
								   -30.000001, -15.000001};
	bool flags[OBJ_NUMBER - 1] = {true, false,
								 false, false};

	float sphereSpeed = 30 / FPS;
	float sleep = 43 * 1000 / (sphereSpeed * FPS);
	float swingSpeeds[OBJ_NUMBER - 1] = {-60 / (138 / sphereSpeed),
										-60 / (138 / sphereSpeed),
										-60 / (138 / sphereSpeed),
										-60 / (138 / sphereSpeed)};
	float initSwingSpeed = -60 / (138 / sphereSpeed);
};

#endif // MAINWINDOW_H
