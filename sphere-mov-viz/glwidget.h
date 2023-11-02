#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMatrix4x4>

#define NEAR_PLANE 0.5f
#define FAR_PLANE 100.0f

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();
protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
private:
	QMatrix4x4 projectionMatrix;
};

#endif // GLWIDGET_H
