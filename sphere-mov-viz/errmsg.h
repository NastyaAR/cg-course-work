#ifndef ERRMSG_H
#define ERRMSG_H

#include <QMessageBox>

typedef enum {
	ERROR,
	WARNING,
	MESSAGE,
} errType;

class ErrMsg
{
public:
	ErrMsg(errType type, const QString &title, const QString &text);
	~ErrMsg() = default;

	void getMessage();
private:
	QString title;
	QString text;
	errType type;
};

#endif // ERRMSG_H
