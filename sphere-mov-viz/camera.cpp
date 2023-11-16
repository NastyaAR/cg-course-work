#include "camera.h"

Camera::Camera() : ViewMatrix(this, &Camera::getViewMatr, nullptr),
				scaleFactor(1.0f)
{
	globalTransform.setToIdentity();
}

void Camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
	if(functions != nullptr) return;
	update();
	program->setUniformValue("viewMatrix", viewMatrix);
}

void Camera::rotate(QQuaternion &rt)
{
	rotation = rt * rotation;
	update();
}

void Camera::rotateX(QQuaternion &rtx)
{
	rotationX = rtx * rotationX;
	update();
}

void Camera::rotateY(QQuaternion &rty)
{
	rotationY = rty * rotationY;
	update();
}

void Camera::transpose(QVector3D &t)
{
	transposition += t;
	update();
}

void Camera::scale(float &s)
{
	scaleFactor *= s;
	update();
}

void Camera::setGlobalTransform(QMatrix4x4 &gt)
{
	globalTransform = gt;
	update();
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
