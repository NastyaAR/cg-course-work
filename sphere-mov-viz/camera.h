#ifndef CAMERA_H
#define CAMERA_H

#include <QQuaternion>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
	Camera();
	void set(QOpenGLShaderProgram* program,
			 QOpenGLFunctions*functions = nullptr);
	void rotate(const QQuaternion &rt);
	void rotateX(const QQuaternion &rtx);
	void rotateY(const QQuaternion &rty);
	void transpose(const QVector3D &t);
	void scale(const float &s);
	void setGlobalTransform(const QMatrix4x4 &gt);

	float getScale();
	QQuaternion getRotation();
	QVector3D getTransposition();
	QMatrix4x4 getGLTransform();

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
};

#endif // CAMERA_H
