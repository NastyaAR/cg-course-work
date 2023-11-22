#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_clicked();
	void generateSignal();
	void handleTimerSignal();

	void on_pushButton_2_clicked();

signals:
	void timerSignal();

private:
	Ui::MainWindow *ui;
	GLWidget *oglw;
	QTimer *timer;
	QVector3D init = {0.0, 0.0, 0.0};
};
#endif // MAINWINDOW_H
