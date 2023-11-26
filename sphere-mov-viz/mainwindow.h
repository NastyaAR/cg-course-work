#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"
#include <iostream>
#include <QShowEvent>

#define RADIUS 5.1
#define FPS 30

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
	void initState(void);

private slots:
	void generateSignal();
	void handleTimerSignal();
	void restore();
	void restore2();
	void restore3();
	void restore4();

	void on_spinBox_valueChanged(int arg1);

signals:
	void timerSignal();
	void initSignal();

private:
	Ui::MainWindow *ui;
	GLWidget *oglw;
	QTimer *timer, *timerSleep1, *timerSleep2, *timerSleep3, *timerSleep4;
	QVector3D init = {0.0, 0.0, 0.0};
	QMatrix4x4 m, m2, m3, m4, spm;
	float _p = 0.000001;
	float p2 = -44.000001;
	float p3 = -29.000001;
	float p4 = -14.000001;
	bool flag1 = true;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;

	float sphereSpeed = 30 / FPS;
	float sleep = 42 * 1000 / (sphereSpeed * FPS);
	float swingSpeed1 = -60 / (138 / sphereSpeed);
	float swingSpeed2 = -60 / (138 / sphereSpeed);
	float swingSpeed3 = -60 / (138 / sphereSpeed);
	float swingSpeed4 = -60 / (138 / sphereSpeed);
	float _swingSpeed1 = -60 / (138 / sphereSpeed);
	float _swingSpeed2 = -60 / (138 / sphereSpeed);
	float _swingSpeed3 = -60 / (138 / sphereSpeed);
	float _swingSpeed4 = -60 / (138 / sphereSpeed);
};

#endif // MAINWINDOW_H
