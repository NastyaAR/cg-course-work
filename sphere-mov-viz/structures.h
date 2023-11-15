#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFramebufferObject>

struct vertex_t
{
	vertex_t() {}
	vertex_t(QVector3D p, QVector2D t, QVector3D n)
		: position(p), textcoord(t), normal(n) {}
	QVector3D position;
	QVector2D textcoord;
	QVector3D normal;
};

struct shadowBuff_t
{
	QOpenGLFramebufferObject *shadowBuff;
	quint64 width;
	quint64 height;
};

#endif // STRUCTURES_H
