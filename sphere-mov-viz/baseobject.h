#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QFile>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "errmsg.h"
#include "structures.h"

class BaseObject
{
public:
	BaseObject() = delete;
	BaseObject(const QString &filename, const QString &texturePath);
	BaseObject(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes, const QString &texturePath);
	~BaseObject();

	void rotate(const QQuaternion &r);
	void translate(const QVector3D &t);
	void scale(const float &s);
	void setGlobalTransform(const QMatrix4x4 &gt);

	void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
protected:
	void free();
	void loadFromFile(const QString &filename);
	void init(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes, const QString &texturePath);
private:
	QOpenGLBuffer vertexesBuffer;
	QOpenGLBuffer indexesBuffer;
	QString texturePath;
	QOpenGLTexture *texture;

	QMatrix4x4 modelMatrix;
	QQuaternion m_Rotate;
	QVector3D m_Translate;
	float m_Scale;
	QMatrix4x4 m_GlobalTransform;

	QVector<GLuint> indexes;
	QVector<vertex_t> vertexes;
};

#endif // BASEOBJECT_H
