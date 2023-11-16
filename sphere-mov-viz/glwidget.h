#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <sstream>

#include "baseobject.h"
#include "errmsg.h"
#include "light.h"
#include "camera.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#define SCALE_FACTOR 0.25f
#define MAX_LIGHT_SOURCES 5

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

	void getShadowMap(int ind, int textInd);

	void initShaders();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
private:
	QMatrix4x4 projectionMatrix;
	QOpenGLShaderProgram shaderProgram;
	QOpenGLShaderProgram shadowShaderProgram;
	QVector2D mousePos;
	QVector3D wheelPlus = QVector3D(0.0f, 0.0f, 0.5);
	QVector3D wheelMinus = QVector3D(0.0f, 0.0f, -0.5);

	QVector<Light *> lights;
	BaseObject *obj1, *obj2;

	float zoom = -15.0f;

	QVector<ShadowBuffer *> shadowBuffers;
	QMatrix4x4 projectionLightMatrix;

	Camera *cam;

};

#endif // GLWIDGET_H
