#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initShaders();
}

void GLWidget::resizeGL(int w, int h)
{
	float param = w / (float) h;
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(90.0f, param, 0.1f, 100.0f);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 viewMatrix;
	viewMatrix.setToIdentity();
	viewMatrix.translate(QVector3D(0.0f, 0.0f, -20.0f));

	BaseObject obj = BaseObject("/home/nastya/sphere.obj", "/home/nastya/cg-course-work/sphere-mov-viz/green.jpg");
	obj.draw(&shaderProgram, context()->functions(), projectionMatrix, viewMatrix);
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



