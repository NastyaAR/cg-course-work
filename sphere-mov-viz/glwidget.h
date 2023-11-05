#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QApplication>

#include "baseobject.h"
#include "errmsg.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f


class GLWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void initShaders();
private:
	QMatrix4x4 projectionMatrix;
	QOpenGLShaderProgram shaderProgram;
};

#endif // GLWIDGET_H
