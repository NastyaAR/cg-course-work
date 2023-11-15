#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	lights.append(new Light(DIRECTIONAL));
	lights[0]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
	lights[0]->Power = 0.9;
	lights[0]->position = QVector4D(0.0f, 0.0f, 10.0f, 1.0f);
	lights[0]->direction = QVector4D(0.0f, 0.0f, -1.0f, 0.0f);

//	lights.append(new Light(DIRECTIONAL));
//	lights[1]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
//	lights[1]->Power = 0.9;
//	lights[1]->direction = QVector4D(1.0f, 1.0f, 1.0f, 0.0f);
//	lights[1]->position = QVector4D(0.0f, 0.0f, 100.0f, 1.0f);

//	lights.append(new Light(POINT));
//	lights[2]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
//	lights[2]->Power = 0.9;
//	lights[2]->direction = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
//	lights[2]->position = QVector4D(100.0f, 0.0f, 1.0f, 1.0f);

	shadowBuffer.width = 1024;
	shadowBuffer.height = 1024;
	projectionLightMatrix.setToIdentity();
	projectionLightMatrix.ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);

	shadowMatrix.setToIdentity();
	shadowMatrix.rotate(30, 1.0, 0.0, 0.0);
	shadowMatrix.rotate(40, 0.0, 1.0, 0.0);

	lightMatrix.setToIdentity();
	auto p = QVector4D(0.0f, 0.0f, 0.0f, 1.0f).normalized();
	auto d = QVector4D(0.0f, 0.0f, -1.0f, 0.0f).normalized();
	lightMatrix.lookAt(p.toVector3D(), (p+d).normalized().toVector3D(), QVector3D(d.x(), d.z(), -d.y()));

	rotation = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 5.0f);

}

GLWidget::~GLWidget()
{
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	delete shadowBuffer.shadowBuff;
}

void GLWidget::initializeGL()
{
	context()->functions()->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	context()->functions()->glEnable(GL_DEPTH_TEST);
	context()->functions()->glEnable(GL_CULL_FACE);

	initShaders();
	shadowBuffer.shadowBuff = new QOpenGLFramebufferObject(shadowBuffer.width, shadowBuffer.height, QOpenGLFramebufferObject::Depth);

	obj1 = new BaseObject("/home/nastya/mys.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj2 = new BaseObject("/home/nastya/cube.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
}

void GLWidget::resizeGL(int w, int h)
{
	float param = w / (float) h;
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(45.0f, param, 0.1f, 1000.0f);
}

void GLWidget::paintGL()
{
	shadowBuffer.shadowBuff->bind();
	context()->functions()->glViewport(0, 0, shadowBuffer.width, shadowBuffer.height);
	context()->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowShaderProgram.bind();
	shadowShaderProgram.setUniformValue("qt_ProjectionLightMatrix", projectionLightMatrix);
	shadowShaderProgram.setUniformValue("qt_ShadowMatrix", lights[0]->lMatrix);
	obj1->draw(&shadowShaderProgram, context()->functions(), projectionLightMatrix, lights[0]->lMatrix, rotation);
	obj2->draw(&shadowShaderProgram, context()->functions(), projectionLightMatrix, lights[0]->lMatrix, rotation);
	shadowShaderProgram.release();

	shadowBuffer.shadowBuff->release();

	GLuint shadowTexture = shadowBuffer.shadowBuff->texture();
	context()->functions()->glActiveTexture(GL_TEXTURE2); // первый слот
	context()->functions()->glBindTexture(GL_TEXTURE_2D, shadowTexture);

	context()->functions()->glViewport(0, 0, this->width(), this->height());
	context()->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 viewMatrix;
	viewMatrix.setToIdentity();
	viewMatrix.translate(QVector3D(0.0f, 0.0f, -15.0f));

	shaderProgram.bind();

	for (int i = 0; i < 1; i++) {
		std::ostringstream oss1;
		std::ostringstream oss2;
		std::ostringstream oss3;
		std::ostringstream oss4;
		std::ostringstream oss5;
		oss1 << "lights[" << i << "].power";
		oss2 << "lights[" << i << "].color";
		oss3 << "lights[" << i << "].pos";
		oss4 << "lights[" << i << "].direction";
		oss5 << "lights[" << i << "].type";
		shaderProgram.setUniformValue(oss1.str().data(), lights[i]->Power);
		shaderProgram.setUniformValue(oss2.str().data(), lights[i]->Clr);
		shaderProgram.setUniformValue(oss3.str().data(), lights[i]->position);
		shaderProgram.setUniformValue(oss4.str().data(), lights[i]->direction);
		shaderProgram.setUniformValue(oss5.str().data(), lights[i]->lType);

	}

	shaderProgram.setUniformValue("numberLights", 1);

	shaderProgram.setUniformValue("qt_ShadowMap0", GL_TEXTURE2 - GL_TEXTURE0);
	shaderProgram.setUniformValue("qt_ProjectionLightMatrix", projectionLightMatrix);
	shaderProgram.setUniformValue("shadowMatrix", lights[0]->lMatrix);
	shaderProgram.setUniformValue("viewMatrix", viewMatrix);

	shaderProgram.setUniformValue("specParam", 10.0f);
	shaderProgram.setUniformValue("ambParam", 0.1f);

//	BaseObject obj = BaseObject("/home/nastya/mys.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj1->draw(&shaderProgram, context()->functions(), projectionMatrix, viewMatrix, rotation);

//	BaseObject cube = BaseObject("/home/nastya/cube.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj2->draw(&shaderProgram, context()->functions(), projectionMatrix, viewMatrix, rotation);

	shaderProgram.release();
}

void GLWidget::initShaders()
{
	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vert.vsh")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции вершинного шейдера!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/frag.fsh")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции фрагментного шейдера!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shaderProgram.link()) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при линковке шейдеров!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shadowShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shadows.vsh")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции вершинного шейдера для теней!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shadowShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shadows.fsh")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции фрагментного шейдера для теней!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shadowShaderProgram.link()) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при линковке шейдеров для теней!").getMessage();
		exit(EXIT_FAILURE);
	}

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
		mousePos = QVector2D(event->localPos());
	event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton) return;
	QVector2D difference = QVector2D(event->localPos()) - mousePos;
	mousePos = QVector2D(event->localPos());

	float angle = difference.length() / 2.0;
	QVector3D axis = QVector3D(difference.y(), difference.x(), 0.0);
	rotation *= QQuaternion::fromAxisAndAngle(axis, angle);

	update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)
		zoom += SCALE_FACTOR;
	else if (event->delta() < 0)
		zoom -= SCALE_FACTOR;

	update();
}



