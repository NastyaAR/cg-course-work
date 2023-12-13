#ifndef TESTCAMERA_H
#define TESTCAMERA_H

#include "camera.h"

class TestCamera : public QObject, public Camera
{
	Q_OBJECT

public:
	TestCamera();
	~TestCamera();

private slots:
	void testScale();
	void testRotate();
	void testTranspose();
};

#endif // TESTCAMERA_H
