#include "light.h"

Light::Light(const lightType type) : type(type),
							Power(this, &Light::getPower, &Light::setPower),
							Clr(this, &Light::getClr, &Light::setClr),
							position(this, &Light::getPos, &Light::setPos),
							direction(this, &Light::getDirect, &Light::setDirect),
							lType(this, &Light::getLightType, &Light::setLightType),
							color(1.0f, 1.0f, 1.0f),
							power(1.0),
							pos(0.0f, 0.0f, 0.0f, 1.0f),
							direct(0.0f, 0.0f, -1.0f, 0.0f),
							lMatrix(this, &Light::getLMatrix, nullptr)
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
	program->setUniformValue(oss1.str().data(), this->Power);
	program->setUniformValue(oss2.str().data(), this->Clr);
	program->setUniformValue(oss3.str().data(), this->position);
	program->setUniformValue(oss4.str().data(), this->direction);
	program->setUniformValue(oss5.str().data(), this->lType);
}
