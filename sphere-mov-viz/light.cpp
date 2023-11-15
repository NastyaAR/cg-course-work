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
