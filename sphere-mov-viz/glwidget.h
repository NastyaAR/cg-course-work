#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>

#include "baseobject.h"
#include "errmsg.h"
#include "light.h"
#include "camera.h"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

#define SCALE_FACTOR 0.25f
#define MAX_LIGHT_SOURCES 10
#define SPHERE_Y -0.65
#define OBJ_NUMBER 5

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();
	QVector<BaseObject *> getObjects();
	BaseObject *getObject(int ind);
	void addLight(QVector3D direction, float power);
	void delLight(QVector4D direction, float power);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void getShadowMap(int ind, int textInd);
	void sendLightsIntoShader(QOpenGLShaderProgram *program);
	void sendShadowIntoShader(QOpenGLShaderProgram *program);
	void sendMaterialIntoShader(QOpenGLShaderProgram *program, int i);
	void initShaders();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
private:
	QMatrix4x4 projectionMatrix;
	QMatrix4x4 projectionLightMatrix;

	QOpenGLShaderProgram shaderProgram;
	QOpenGLShaderProgram shadowShaderProgram;

	QVector2D mousePos;
	QVector3D wheelPlus = QVector3D(0.0f, 0.0f, 0.5);
	QVector3D wheelMinus = QVector3D(0.0f, 0.0f, -0.5);

	QVector<Light *> lights;
	QVector<BaseObject *> objects;
	QVector<ShadowBuffer *> shadowBuffers;

	float zoom = -15.0f;
	Camera *cam;

	int shadowTextures[10] = {GL_TEXTURE2, GL_TEXTURE3,
							 GL_TEXTURE4, GL_TEXTURE5,
							 GL_TEXTURE6, GL_TEXTURE7,
							 GL_TEXTURE8, GL_TEXTURE9,
							 GL_TEXTURE10, GL_TEXTURE11};
	QString texturePaths[5] = {
		"/home/nastya/cg-course-work/textures/green.jpg",
		"/home/nastya/cg-course-work/textures/pink2.jpg",
		"/home/nastya/cg-course-work/textures/pink2.jpg",
		"/home/nastya/cg-course-work/textures/pink2.jpg",
		"/home/nastya/cg-course-work/textures/pink2.jpg",
	};
	QString objPaths[5] = {
		"/home/nastya/cg-course-work/objects/1.obj",
		"/home/nastya/cg-course-work/objects/2.obj",
		"/home/nastya/cg-course-work/objects/3.obj",
		"/home/nastya/cg-course-work/objects/4.obj",
		"/home/nastya/cg-course-work/objects/5.obj",
	};

	int cur_lights = 1;
};

#endif // GLWIDGET_H
