#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initShaders();
}

void GLWidget::resizeGL(int w, int h)
{
	float param = w / (float) h;
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(45.0f, param, 0.1f, 100.0f);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 viewMatrix;
	viewMatrix.setToIdentity();
	viewMatrix.translate(QVector3D(0.0f, 0.0f, zoom));

	BaseObject obj = BaseObject("/home/nastya/mys.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj.draw(&shaderProgram, context()->functions(), projectionMatrix, viewMatrix, rotation);
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



