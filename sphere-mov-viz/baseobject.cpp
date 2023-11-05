#include "baseobject.h"

BaseObject::BaseObject(const QString &filename, const QString &texturePath) :
	vertexesBuffer(QOpenGLBuffer::VertexBuffer),
	indexesBuffer(QOpenGLBuffer::IndexBuffer),
	texturePath(texturePath),
	texture(nullptr)
{
	loadFromFile(filename);
	init(vertexes, indexes, texturePath);
}

BaseObject::BaseObject(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes,
					   const QString &texturePath) :
	vertexesBuffer(QOpenGLBuffer::VertexBuffer),
	indexesBuffer(QOpenGLBuffer::IndexBuffer),
	texturePath(texturePath),
	texture(nullptr)
{
	this->vertexes = vertexes;
	this->indexes = indexes;
	init(this->vertexes, this->indexes, this->texturePath);
}

BaseObject::~BaseObject()
{
	free();
}

void BaseObject::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix)
{
//	modelMatrix.translate(QVector3D(0.0f, 0.0f, 10.0f));
//	modelMatrix.scale(0.5);

	texture->bind(0);
	program->setUniformValue("qt_Texture0", 0);

	vertexesBuffer.bind();

	auto vertexesLocation = program->attributeLocation("qt_Vertex");
	program->enableAttributeArray(vertexesLocation);
	program->setAttributeBuffer(vertexesLocation, GL_FLOAT, 0, 3, sizeof(vertex_t));

	auto texLocation = program->attributeLocation("qt_MultiTexCoord0");
	program->enableAttributeArray(texLocation);
	program->setAttributeBuffer(texLocation, GL_FLOAT, sizeof(QVector3D), 2, sizeof(vertex_t));

	indexesBuffer.bind();

	functions->glDrawElements(GL_TRIANGLES, indexesBuffer.size(), GL_UNSIGNED_INT, 0);

	indexesBuffer.release();
	vertexesBuffer.release();
	texture->release();
}

void BaseObject::free()
{
//	indexes.clear();
//	vertexes.clear();

	if (vertexesBuffer.isCreated()) {
		vertexesBuffer.destroy();
	}
	if (indexesBuffer.isCreated()) {
		indexesBuffer.destroy();
	}
	if (texture && texture->isCreated()) {
		delete texture;
		texture = nullptr;
	}
}

void BaseObject::loadFromFile(const QString &filename)
{
	QVector<QVector2D> textureCoordinates;
	QVector<QVector3D> normals;
	QVector<QVector3D> coordinates;

	QFile src(filename);
	if (!src.exists()) {
		ErrMsg(ERROR, "Ошибка", "Не найден файл модели!");
		return;
	}

	src.open(QFile::ReadOnly);
	QTextStream stream(&src);
	while (!stream.atEnd()) {
		QString curStr = stream.readLine();
		if (curStr == "\n")
			continue;
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
			for (int i = 1; i < 4; i++) {
				QStringList frags = strs[i].split("/");
				vertex_t new_v = vertex_t(coordinates[frags[0].toInt() - 1],
						textureCoordinates[frags[1].toInt() - 1],
						normals[frags[2].toInt() - 1]);
				vertexes.append(new_v);
				indexes.append(indexes.size());
			}
		}
	}

	src.close();
}

void BaseObject::init(const QVector<vertex_t> &vertexes, const QVector<GLuint> &indexes, const QString &texturePath)
{
	free();

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


