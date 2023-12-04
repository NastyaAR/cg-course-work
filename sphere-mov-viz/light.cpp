#include "light.h"

Light::Light(const lightType type) : type(type),
							color(1.0f, 1.0f, 1.0f),
							power(1.0),
							pos(0.0f, 0.0f, 0.0f, 1.0f),
							direct(0.0f, 0.0f, -1.0f, 0.0f),
							isUsed(false)
{
	lightMatrix.setToIdentity();
	lightMatrix.lookAt(pos.normalized().toVector3D(), (pos.normalized() + direct).normalized().toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y()));
}

void Light::sendToShader(QOpenGLShaderProgram *program, int i)
{
	std::ostringstream oss1;
	std::ostringstream oss2;
	std::ostringstream oss3;
	std::ostringstream oss4;
	std::ostringstream oss5;
	oss1 << "lights[" << i << "].power";
	oss2 << "lights[" << i << "].color";
	oss3 << "lights[" << i << "].pos";
	oss4 << "lights[" << i << "].direction";
	oss5 << "lights[" << i << "].type";
	program->setUniformValue(oss1.str().data(), this->power);
	program->setUniformValue(oss2.str().data(), this->color);
	program->setUniformValue(oss3.str().data(), this->pos);
	program->setUniformValue(oss4.str().data(), this->direct);
	program->setUniformValue(oss5.str().data(), this->type);
}

void Light::setDirect(QVector4D d)
{
	direct = d.normalized();
	lightMatrix.setToIdentity();
	lightMatrix.lookAt(pos.normalized().toVector3D(),
					   (pos.normalized() + direct).normalized().toVector3D(),
					   QVector3D(direct.x(), direct.z(), -direct.y()));
}

void Light::setPos(QVector4D p)
{
	pos = p;
	lightMatrix.setToIdentity();
	lightMatrix.lookAt(pos.normalized().toVector3D(),
					   (pos.normalized() + direct).normalized().toVector3D(),
					   QVector3D(direct.x(), direct.z(), -direct.y()));
}
