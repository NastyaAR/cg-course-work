#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
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

void GLWidget::resizeGL(int width, int height)
{
	float param = (float) width / height;
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(45, param, NEAR_PLANE, FAR_PLANE);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::initShaders()
{
	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertexShader.vert")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции вершинного шейдера!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragmentShader.frag")) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при компиляции фрагментного шейдера!").getMessage();
		exit(EXIT_FAILURE);
	}

	if (!shaderProgram.link()) {
		ErrMsg(ERROR, "Ошибка", "Ошибка при линковке шейдеров!").getMessage();
		exit(EXIT_FAILURE);
	}

}



