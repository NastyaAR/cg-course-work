#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QApplication>

#include "errmsg.h"

#define NEAR_PLANE 0.5f
#define FAR_PLANE 100.0f


class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();
protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void initShaders();
private:
	QMatrix4x4 projectionMatrix;
	QOpenGLShaderProgram shaderProgram;
	QImage texImg = QImage("/home/nastya/cg-course-work/sphere-mov-viz/pink.png");
	QOpenGLTexture *texture = new QOpenGLTexture(texImg.mirrored());
};

#endif // GLWIDGET_H
