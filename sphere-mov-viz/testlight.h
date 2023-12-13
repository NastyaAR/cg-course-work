#ifndef TESTLIGHT_H
#define TESTLIGHT_H

#include <QObject>

class TestLight : public QObject
{
	Q_OBJECT
public:
	TestLight();
	~TestLight();
private slots:
	void testSetDirect();
	void testSetClr();
	void testSetPower();
	void testSetUsed();
};

#endif // TESTLIGHT_H
