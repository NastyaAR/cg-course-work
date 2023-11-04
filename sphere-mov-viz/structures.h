#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QVector2D>
#include <QVector3D>

typedef struct vertex {
	QVector3D coordinate;
	QVector2D textureCoordinate;
	QVector3D normal;
} vertex_t;

#endif // STRUCTURES_H
