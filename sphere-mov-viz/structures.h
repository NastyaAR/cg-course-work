#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFramebufferObject>
#include "property.h"

struct vertex_t
{
	vertex_t() {}
	vertex_t(QVector3D p, QVector2D t, QVector3D n)
		: position(p), textcoord(t), normal(n) {}
	QVector3D position;
	QVector2D textcoord;
	QVector3D normal;
};

class ShadowBuffer
{
public:
	ShadowBuffer(int w, int h) : width(w), height(h), shadowBuff(nullptr) {};
	~ShadowBuffer() { delete shadowBuff; }
	QOpenGLFramebufferObject *shadowBuff;
	quint64 width;
	quint64 height;
};

struct materialProperties_t
{
	float ambParam;
	float diffParam;
	float specParam;
};

#endif // STRUCTURES_H
