#include "baseobject.h"

BaseObject::BaseObject(const QString &filename, const QString &texturePath) :
	vertexesBuffer(QOpenGLBuffer::VertexBuffer),
	indexesBuffer(QOpenGLBuffer::IndexBuffer),
	texturePath(texturePath),
	texture(nullptr)
{
}

BaseObject::BaseObject(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes, const QString &texturePath) :
	vertexesBuffer(QOpenGLBuffer::VertexBuffer),
	indexesBuffer(QOpenGLBuffer::IndexBuffer),
	texturePath(texturePath),
	texture(nullptr)
{

}

BaseObject::~BaseObject()
{
	free();
}

void BaseObject::free()
{
	if (vertexesBuffer.isCreated()) {
		vertexesBuffer.destroy();
	}
	if (indexesBuffer.isCreated()) {
		indexesBuffer.destroy();
	}
	if (texture->isCreated()) {
		delete texture;
		texture = nullptr;
	}
}

void BaseObject::loadFromFile(const QString &filename)
{
	QVector<QVector3D> coordinates;
	QVector<GLuint> indexes;
	QVector<QVector2D> textureCoordinates;
	QVector<QVector3D> normals;

	QFile src(filename);
	if (!src.exists()) {
		ErrMsg(ERROR, "Ошибка", "Не найден файл модели!");
		return;
	}

	QVector<vertex_t> vertexes;

	src.open(QFile::ReadOnly);
	QTextStream stream(&src);
	while (!stream.atEnd()) {
		QString curStr = stream.readLine();
		QStringList strs = curStr.split(" ");
		if (strs[0] == "#" || strs[0] == "mtllib" || strs[0] == "o") {
			continue;
		} else if (strs[0] == "v") {
			coordinates.append(QVector3D(strs[1].toFloat(), strs[2].toFloat(), strs[3].toFloat()));
		} else if (strs[0] == "vt") {
			textureCoordinates.append(QVector2D(strs[1].toFloat(), strs[2].toFloat()));
		} else if (strs[0] == "vn") {
			normals.append(QVector3D(strs[1].toFloat(), strs[2].toFloat(), strs[3].toFloat()));
		} else if (strs[0] == "f") {
			for (int i = 0; i < 4; i++) {
				QStringList frags = strs[i].split("/");
				vertex_t vertex = {.coordinate = coordinates[frags[0].toInt() - 1],
								  .textureCoordinate = textureCoordinates[frags[1].toInt() - 1],
								  .normal = normals[frags[2].toInt() - 1]};
				vertexes.append(vertex);
				indexes.append(indexes.size());
			}
		}
	}

	src.close();
}

void BaseObject::init(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes, const QString &texturePath)
{
	vertexesBuffer.create();
	vertexesBuffer.bind();
	vertexesBuffer.allocate(vertexes.constData(), sizeof(vertex_t) * vertexes.size());
	vertexesBuffer.release();

	indexesBuffer.create();
	indexesBuffer.bind();
	indexesBuffer.allocate(indexes.constData(), sizeof(GLuint) * indexes.size());
	indexesBuffer.release();

	texture = new QOpenGLTexture(QImage(texturePath).mirrored());
	texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);

	modelMatrix.setToIdentity();
}


