#include "light.h"

Light::Light(const lightType type) : type(type),
							ambClr(this, &Light::getAmbClr, &Light::setAmbClr),
							diffClr(this, &Light::getDiffClr, &Light::setDiffClr),
							specClr(this, &Light::getSpecClr, &Light::setSpecClr),
							position(this, &Light::getPos, &Light::setPos),
							direction(this, &Light::getDirect, &Light::setDirect),
							lType(this, &Light::getLightType, &Light::setLightType),
							ambColor(1.0f, 1.0f, 1.0f),
							diffColor(1.0f, 1.0f, 1.0f),
							specColor(1.0f, 1.0f, 1.0f),
							pos(0.0f, 0.0f, 0.0f, 1.0f),
							direct(0.0f, 0.0f, -1.0f, 0.0f),
							lMatrix(this, &Light::getLMatrix, nullptr)
{
	lightMatrix.setToIdentity();
	lightMatrix.lookAt(pos.toVector3D(), (pos + direct).toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y()));
}
