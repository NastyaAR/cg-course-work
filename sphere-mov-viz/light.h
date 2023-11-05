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
	const Property<QVector3D, Light> ambClr;
	const Property<QVector3D, Light> diffClr;
	const Property<QVector3D, Light> specClr;
	const Property<QVector4D, Light> position;
	const Property<QVector4D, Light> direction;
	const Property<lightType, Light> lType;
	const Property<QMatrix4x4, Light> lMatrix;

	Light(const lightType type);
private:
	QVector4D pos;

	QVector3D ambColor;
	QVector3D diffColor;
	QVector3D specColor;

	QVector4D direct;

	lightType type;

	void setAmbClr(QVector3D clr) { ambColor = clr; };
	void setDiffClr(QVector3D clr) { diffColor = clr; };
	void setSpecClr(QVector3D clr) { specColor = clr; };
	void setDirect(QVector4D d) { direct = d.normalized(); 	lightMatrix.setToIdentity();
								  lightMatrix.lookAt(pos.toVector3D(), (pos + direct).toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y())); };
	void setPos(QVector4D p) { pos = p; 	lightMatrix.setToIdentity();
							   lightMatrix.lookAt(pos.toVector3D(), (pos + direct).toVector3D(), QVector3D(direct.x(), direct.z(), -direct.y())); };
	void setLightType(lightType t) { type = t; };

	QVector3D getAmbClr() { return ambColor; };
	QVector3D getDiffClr() { return diffColor; };
	QVector3D getSpecClr() { return specColor; };
	QVector4D getDirect() { return direct; };
	QVector4D getPos() { return pos; };
	lightType getLightType() { return type; };
	QMatrix4x4 getLMatrix() { return lightMatrix; };

	QMatrix4x4 lightMatrix;
};

#endif // LIGHT_H
