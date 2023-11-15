#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#include "property.h"

typedef enum {
	DIRECTIONAL,
	POINT,
} lightType;

class Light
{
public:
	const Property<float, Light> Power;
	const Property<QVector3D, Light> Clr;
	const Property<QVector4D, Light> position;
	const Property<QVector4D, Light> direction;
	const Property<lightType, Light> lType;
	const Property<QMatrix4x4, Light> lMatrix;

	Light(const lightType type);
	Light() = default;
private:
	float power;
	QVector4D pos;
	QVector3D color;
	QVector4D direct;
	lightType type;

	void setClr(QVector3D clr) { color = clr; };
	void setDirect(QVector4D d) { direct = d.normalized(); 	lightMatrix.setToIdentity();
								  lightMatrix.lookAt(pos.normalized().toVector3D(), (pos.normalized() + direct).normalized().toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y())); };
	void setPos(QVector4D p) { pos = p; 	lightMatrix.setToIdentity();
							   lightMatrix.lookAt(pos.normalized().toVector3D(), (pos.normalized() + direct).normalized().toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y())); };
	void setLightType(lightType t) { type = t; };
	void setPower(float pwr) { power = pwr; };

	QVector3D getClr() { return color; };
	QVector4D getDirect() { return direct; };
	QVector4D getPos() { return pos; };
	lightType getLightType() { return type; };
	QMatrix4x4 getLMatrix() { return lightMatrix; };
	float getPower() { return power; };

	QMatrix4x4 lightMatrix;
};

#endif // LIGHT_H
