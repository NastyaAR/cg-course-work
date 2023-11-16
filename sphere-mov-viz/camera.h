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

	void draw(QOpenGLShaderProgram* program, QOpenGLFunctions*functions = nullptr);
	void rotate(QQuaternion &rt);
	void rotateX(QQuaternion &rtx);
	void rotateY(QQuaternion &rty);
	void transpose(QVector3D &t);
	void scale(float &s);
	void setGlobalTransform(QMatrix4x4 &gt);

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
