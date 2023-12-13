#include <QTest>
#include "testlight.h"
#include "light.h"

TestLight::TestLight()
{

}

TestLight::~TestLight()
{

}

void TestLight::testSetClr()
{
	QVector3D expected(0.0, 0.0, 0.0);
	auto light = Light();
	light.setClr(expected);
	auto actual = light.getClr();
	QVERIFY2(qFuzzyCompare(actual, expected), "Векторы не равны");
}

void TestLight::testSetPower()
{
	float expected = 5.0;
	auto light = Light();
	light.setPower(expected);
	auto actual = light.getPower();
	QVERIFY2(qFuzzyCompare(actual, expected), "Мощности не равны");
}

void TestLight::testSetUsed()
{
	bool expected = true;
	auto light = Light();
	light.setUsed(expected);
	auto actual = light.getUsed();
	QVERIFY2(expected == actual, "Мощности не равны");
}

void TestLight::testSetDirect()
{
	auto light = Light();
	QVector4D dir(0.0, 5.0, 0.0, 0.0);
	light.setDirect(dir);
	auto actual = light.getDirect();
	QVector4D expected(0.0, 1.0, 0.0, 0.0);
	QVERIFY2(expected == actual, "Векторы не равны");
}
