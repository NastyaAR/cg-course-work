#include "camera.h"

Camera::Camera() : scaleFactor(1.0f)
{
	globalTransform.setToIdentity();
}

void Camera::set(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
	if(functions != nullptr) return;
	update();
	program->setUniformValue("viewMatrix", viewMatrix);
}

void Camera::rotate(const QQuaternion &rt)
{
	rotation = rt * rotation;
	update();
}

void Camera::rotateX(const QQuaternion &rtx)
{
	rotationX = rtx * rotationX;
	update();
}

void Camera::rotateY(const QQuaternion &rty)
{
	rotationY = rty * rotationY;
	update();
}

void Camera::transpose(const QVector3D &t)
{
	transposition += t;
	update();
}

void Camera::scale(const float &s)
{
	scaleFactor *= s;
	update();
}

void Camera::setGlobalTransform(const QMatrix4x4 &gt)
{
	globalTransform = gt;
	update();
}

float Camera::getScale()
{
	return scaleFactor;
}

QQuaternion Camera::getRotation()
{
	return rotation;
}

QVector3D Camera::getTransposition()
{
	return transposition;
}

void Camera::update()
{
	rotation = rotationX * rotationY;

	viewMatrix.setToIdentity();
	viewMatrix.translate(transposition);
	viewMatrix.rotate(rotation);
	viewMatrix.scale(scaleFactor);
	viewMatrix *= globalTransform.inverted();
}
