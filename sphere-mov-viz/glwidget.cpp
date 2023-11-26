#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	lights.append(new Light(DIRECTIONAL));
	lights[0]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
	lights[0]->Power = 0.9;
	lights[0]->position = QVector4D(0.0f, 0.0f, 10.0f, 1.0f);
	lights[0]->direction = QVector4D(0.0f, -1.0f, 0.0f, 0.0f);

	shadowBuffers.append(new ShadowBuffer(1024, 1024));
	shadowBuffers.append(new ShadowBuffer(1024, 1024));
	projectionLightMatrix.setToIdentity();
	projectionLightMatrix.ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);

	cam = new Camera;
	cam->transpose(QVector3D(0.0f, 0.0f, -10.0f));
}

GLWidget::~GLWidget()
{
	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	for (int i = 0; i < shadowBuffers.size(); i++) {
		delete shadowBuffers[i];
	}

	delete cam;

	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}

void GLWidget::initializeGL()
{
	context()->functions()->glClearColor(1.0f, 0.85f, 0.73f, 1.0f);
	context()->functions()->glEnable(GL_DEPTH_TEST);
	context()->functions()->glEnable(GL_CULL_FACE);

	initShaders();

	shadowBuffers[0]->shadowBuff = new QOpenGLFramebufferObject(shadowBuffers[0]->width, shadowBuffers[0]->height, QOpenGLFramebufferObject::Depth);
	shadowBuffers[1]->shadowBuff = new QOpenGLFramebufferObject(shadowBuffers[1]->width, shadowBuffers[1]->height, QOpenGLFramebufferObject::Depth);

	obj1 = new BaseObject("/home/nastya/cg-course-work/objects/1.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj2 = new BaseObject("/home/nastya/cg-course-work/objects/2.obj", "/home/nastya/cg-course-work/sphere-mov-viz/pink2.jpg");
	obj3 = new BaseObject("/home/nastya/cg-course-work/objects/3.obj", "/home/nastya/cg-course-work/sphere-mov-viz/pink2.jpg");
	obj4 = new BaseObject("/home/nastya/cg-course-work/objects/4.obj", "/home/nastya/cg-course-work/sphere-mov-viz/pink2.jpg");
	obj5 = new BaseObject("/home/nastya/cg-course-work/objects/5.obj", "/home/nastya/cg-course-work/sphere-mov-viz/pink2.jpg");

	obj1->translate(QVector3D(0.0, -0.55, 0.0));
	obj5->scale(0.95);
	obj4->scale(0.95);
	obj3->scale(0.95);
	obj2->scale(0.95);

	objects.push_back(obj1);
	objects.push_back(obj2);
	objects.push_back(obj3);
	objects.push_back(obj4);
	objects.push_back(obj5);
}

void GLWidget::resizeGL(int w, int h)
{
	float param = w / (float) h;
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(45.0f, param, NEAR_PLANE, FAR_PLANE);
}

void GLWidget::getShadowMap(int ind, int textInd)
{
	shadowBuffers[ind]->shadowBuff->bind();
	context()->functions()->glViewport(0, 0, shadowBuffers[ind]->width, shadowBuffers[ind]->height);
	context()->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowShaderProgram.bind();
	shadowShaderProgram.setUniformValue("qt_ProjectionLightMatrix", projectionLightMatrix);
	shadowShaderProgram.setUniformValue("qt_ShadowMatrix", lights[ind]->lMatrix);
	for (auto obj : objects)
		obj->draw(&shadowShaderProgram, context()->functions());
	shadowShaderProgram.release();

	shadowBuffers[ind]->shadowBuff->release();

	GLuint shadowTexture = shadowBuffers[ind]->shadowBuff->texture();
	context()->functions()->glActiveTexture(textInd);
	context()->functions()->glBindTexture(GL_TEXTURE_2D, shadowTexture);
}

void GLWidget::sendLightsIntoShader(QOpenGLShaderProgram *program)
{
	for (int i = 0; i < lights.size(); i++)
		lights[i]->sendToShader(program, i);
}

void GLWidget::paintGL()
{
	getShadowMap(0, GL_TEXTURE2);
//	getShadowMap(1, GL_TEXTURE3);

	context()->functions()->glViewport(0, 0, this->width(), this->height());
	context()->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.bind();
	sendLightsIntoShader(&shaderProgram);
	cam->set(&shaderProgram);

	shaderProgram.setUniformValue("numberLights", 1);
	shaderProgram.setUniformValue("numberShadows", 1);
	shaderProgram.setUniformValue("qt_ShadowMaps0[0]", GL_TEXTURE2 - GL_TEXTURE0);
	shaderProgram.setUniformValue("qt_ShadowMaps0[1]", GL_TEXTURE2 - GL_TEXTURE0);
	shaderProgram.setUniformValue("qt_ProjectionLightMatrix", projectionLightMatrix);
	shaderProgram.setUniformValue("shadowMatrixes[0]", lights[0]->lMatrix);
	shaderProgram.setUniformValue("shadowMatrixes[1]", lights[0]->lMatrix);
	shaderProgram.setUniformValue("qt_ProjectionMatrix", projectionMatrix);

	shaderProgram.setUniformValue("specParam", 10.0f);
	shaderProgram.setUniformValue("ambParam", 0.1f);

	for (auto obj : objects)
		obj->draw(&shaderProgram, context()->functions());

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
	if (event->buttons() == Qt::LeftButton) {
		QVector2D diffpos = QVector2D(event->localPos()) - mousePos;
		mousePos = QVector2D(event->localPos());

		float angleX = diffpos.y() / 2.0f;
		float angleY = diffpos.x() / 2.0f;
		auto rtx = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX);
		auto rty = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY);
		cam->rotateX(rtx);
		cam->rotateY(rty);
		update();
	}
	event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{

	if(event->delta() > 0) cam->transpose(wheelPlus);
	else if(event->delta() < 0) cam->transpose(wheelMinus);
	update();
	event->accept();
}



