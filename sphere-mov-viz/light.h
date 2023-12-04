#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <sstream>

typedef enum {
	DIRECTIONAL,
} lightType;

class Light
{
public:
	void setClr(QVector3D clr) { color = clr; };
	void setDirect(QVector4D d);
	void setPos(QVector4D p);
	void setLightType(lightType t) { type = t; };
	void setPower(float pwr) { power = pwr; };
	void setUsed(bool val) { isUsed = val; };

	QVector3D getClr() { return color; };
	QVector4D getDirect() { return direct; };
	QVector4D getPos() { return pos; };
	lightType getLightType() { return type; };
	QMatrix4x4 getLMatrix() { return lightMatrix; };
	float getPower() { return power; };
	bool getUsed() { return isUsed; };

	Light(const lightType type);
	Light() = default;

	void sendToShader(QOpenGLShaderProgram *program, int i);
private:
	float power;
	QVector4D pos;
	QVector3D color;
	QVector4D direct;
	lightType type;
	bool isUsed;

	QMatrix4x4 lightMatrix;
};

#endif // LIGHT_H
