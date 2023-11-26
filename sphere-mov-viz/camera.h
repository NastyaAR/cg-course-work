#ifndef CAMERA_H
#define CAMERA_H

#include <QQuaternion>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

#include "property.h"

class Camera
{
public:
	Camera();

	const Property<QMatrix4x4, Camera> ViewMatrix;

	void set(QOpenGLShaderProgram* program, QOpenGLFunctions*functions = nullptr);
	void rotate(const QQuaternion &rt);
	void rotateX(const QQuaternion &rtx);
	void rotateY(const QQuaternion &rty);
	void transpose(const QVector3D &t);
	void scale(const float &s);
	void setGlobalTransform(const QMatrix4x4 &gt);

protected:
	void update();

private:
	QQuaternion rotation;
	QQuaternion rotationX;
	QQuaternion rotationY;
	QVector3D transposition;
	float scaleFactor;
	QMatrix4x4 globalTransform;
	QMatrix4x4 viewMatrix;

	QMatrix4x4 getViewMatr() { return viewMatrix; }
};

#endif // CAMERA_H
