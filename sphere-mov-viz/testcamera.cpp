#include "testcamera.h"

#include <QtTest>

TestCamera::TestCamera()
{

}

TestCamera::~TestCamera()
{

}


void TestCamera::testScale()
{
	auto cam = Camera();
	cam.scale(5.0);
	float actual = cam.getScale();
	float expected = 5.0;
	QVERIFY2(qFuzzyCompare(actual, expected), "Числа не равны");
}

void TestCamera::testRotate()
{
	auto cam = Camera();
	cam.rotateX(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), 30));
	auto actual = cam.getRotation();
	auto expected = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), 30);
	QVERIFY2(qFuzzyCompare(actual, expected), "Кватернионы не равны");
}

void TestCamera::testTranspose()
{
	auto cam = Camera();
	cam.transpose(QVector3D(0.0, 1.0, 0.0));
	auto actual = cam.getTransposition();
	auto expected = QVector3D(0.0, 1.0, 0.0);
	QVERIFY2(qFuzzyCompare(actual, expected), "Векторы не равны");
}
