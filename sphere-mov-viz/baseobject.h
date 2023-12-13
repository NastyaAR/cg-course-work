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
	BaseObject(const QString &filename,
			   const QString &texturePath,
			   materialProperties_t mat);
	BaseObject(const QVector<vertex_t> &vertexes,
			   const QVector<GLuint> &indexes,
			   const QString &texturePath,
			   materialProperties_t mat);
	~BaseObject();

	void rotate(const QQuaternion &r);
	void translate(const QVector3D &t);
	void scale(const float &s);
	void setGlobalTransform(const QMatrix4x4 &gt);
	void resetTransformations();

	void draw(QOpenGLShaderProgram *program,
			  QOpenGLFunctions *functions);
	materialProperties_t getMaterial();
	void changeTexture(const QString &texturePath);
	void changeMaterial(int n, float val);
protected:
	void free();
	void loadFromFile(const QString &filename);
	void init(const QVector<vertex_t> &vertexes,
			  const QVector<GLuint> &indexes,
			  const QString &texturePath);
private:
	QOpenGLBuffer vertexesBuffer;
	QOpenGLBuffer indexesBuffer;
	QString texturePath;
	QOpenGLTexture *texture;

	QQuaternion rotation;
	QVector3D transposition;
	float scaleFactor;
	QMatrix4x4 globalTransform;

	QVector<GLuint> indexes;
	QVector<vertex_t> vertexes;

	materialProperties_t material;
};

#endif // BASEOBJECT_H
