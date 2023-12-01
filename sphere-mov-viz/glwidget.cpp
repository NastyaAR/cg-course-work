#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	for (int i = 0; i < 10; i++) {
		lights.append(new Light(DIRECTIONAL));
		lights[i]->Clr = QVector3D(1.0f, 1.0f, 1.0f);
		lights[i]->Power = 0.9;
		lights[i]->position = QVector4D(0.0f, 0.0f, 10.0f, 1.0f);
		lights[i]->direction = QVector4D(0.0f, -1.0f, 0.0f, 0.0f);
		lights[i]->used = false;
	}

	lights[0]->used = true;
	lights[2]->direction = QVector4D(0.0f, 1.0f, 0.0f, 0.0f);

	for (int i = 0; i < 10; i++) {
		shadowBuffers.append(new ShadowBuffer(1024, 1024));
	}
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

QVector<BaseObject *> GLWidget::getObjects()
{
	return objects;
}

BaseObject *GLWidget::getObject(int ind)
{
	return objects[ind];
}

void GLWidget::addLight(QVector3D direction, float power)
{
	int ind = -1;
	for (int i = 0; i < lights.size(); i++)
		if (lights[i]->used == false) {
			ind = i;
			break;
		}
	if (ind == -1)
		return;
	lights[ind]->used = true;
	lights[ind]->direction = QVector4D(direction, 0.0);
	lights[ind]->Power = power;
	cur_lights++;
}

static bool areVectorsEqual(const QVector4D vec1, const QVector4D vec2)
{
	if (!qFuzzyCompare(vec1.x(), vec2.x()))
		return false;
	if (!qFuzzyCompare(vec1.y(), vec2.y()))
		return false;
	if (!qFuzzyCompare(vec1.z(), vec2.z()))
		return false;
	if (!qFuzzyCompare(vec1.w(), vec2.w()))
		return false;

	return true;
}

void GLWidget::delLight(QVector4D direction, float power)
{
	for (int i = 0; i < lights.size(); i++)
		if (lights[i]->used == true && areVectorsEqual(lights[i]->direction, direction)
				&& qFuzzyCompare(lights[i]->Power, power)) {
			lights[i]->used = false;
			cur_lights--;
			break;
		}
}

void GLWidget::initializeGL()
{
	context()->functions()->glClearColor(1.0f, 0.85f, 0.73f, 1.0f);
	context()->functions()->glEnable(GL_DEPTH_TEST);
	context()->functions()->glEnable(GL_CULL_FACE);

	initShaders();
	for (int i = 0; i < lights.size(); i++)
		shadowBuffers[i]->shadowBuff = new QOpenGLFramebufferObject(shadowBuffers[i]->width,
													shadowBuffers[i]->height,
													QOpenGLFramebufferObject::Depth);

	materialProperties_t material = {0.1f, 0.9f, 10.0f};

	for (int i = 0; i < OBJ_NUMBER; i++)
		objects.push_back(new BaseObject(objPaths[i], texturePaths[i], material));

	objects[0]->translate(QVector3D(0.0, SPHERE_Y, 0.0));
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
	int j = 0;
	for (int i = 0; i < lights.size(); i++)
		if (lights[i]->used == true) {
			lights[i]->sendToShader(program, j);
			j++;
		}
}

void GLWidget::sendShadowIntoShader(QOpenGLShaderProgram *program)
{
	int j = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->used == true) {
			std::ostringstream oss1;
			std::ostringstream oss2;
			oss1 << "qt_ShadowMaps0[" << j << "]";
			oss2 << "shadowMatrixes[" << j << "]";
			program->setUniformValue(oss1.str().data(), shadowTextures[i] - GL_TEXTURE0);
			program->setUniformValue(oss2.str().data(), lights[i]->lMatrix);
			j++;
		}
	}
}

void GLWidget::sendMaterialIntoShader(QOpenGLShaderProgram *program, int i)
{
	program->setUniformValue("specParam", objects[i]->getMaterial().specParam);
	program->setUniformValue("ambParam", objects[i]->getMaterial().ambParam);
	program->setUniformValue("diffParam", objects[i]->getMaterial().diffParam);
}

void GLWidget::paintGL()
{
	for (int i = 0; i < lights.size(); i++)
		if (lights[i]->used == true)
			getShadowMap(i, shadowTextures[i]);
	context()->functions()->glViewport(0, 0, this->width(), this->height());
	context()->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderProgram.bind();
	sendLightsIntoShader(&shaderProgram);
	cam->set(&shaderProgram);
	shaderProgram.setUniformValue("numberLights", cur_lights);
	shaderProgram.setUniformValue("numberShadows", cur_lights);
	shaderProgram.setUniformValue("qt_ProjectionLightMatrix", projectionLightMatrix);
	sendShadowIntoShader(&shaderProgram);
	shaderProgram.setUniformValue("qt_ProjectionMatrix", projectionMatrix);
	sendMaterialIntoShader(&shaderProgram, 0);
	objects[0]->draw(&shaderProgram, context()->functions());
	sendMaterialIntoShader(&shaderProgram, 1);
	for (int i = 0; i < objects.size(); i++)
		objects[i]->draw(&shaderProgram, context()->functions());
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



