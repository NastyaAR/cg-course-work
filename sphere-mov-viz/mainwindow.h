#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"
#include <iostream>
#include <QShowEvent>

#define RADIUS 5.1

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
	void showEvent(QShowEvent *event);

private slots:
	void on_pushButton_clicked();
	void generateSignal();
	void handleTimerSignal();
	void restore();
	void restore2();
	void restore3();
	void restore4();
	void on_pushButton_2_clicked();

signals:
	void timerSignal();
	void initSignal();

private:
	Ui::MainWindow *ui;
	GLWidget *oglw;
	QTimer *timer, *timerSleep1, *timerSleep2, *timerSleep3, *timerSleep4;
	QVector3D init = {0.0, 0.0, 0.0};
	QMatrix4x4 m, m2, m3, m4;
	float _p = 0.000001;
	float p2 = -46.000001;
	float p3 = -31.000001;
	float p4 = -16.000001;
	int sp = -1.0;
	int sp2 = -1.0;
	int sp3 = -1.0;
	int sp4 = -1.0;
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;
	int c = 3;
};
#endif // MAINWINDOW_H
