#include "errmsg.h"

ErrMsg::ErrMsg(errType type, const QString &title, const QString &text) : title(title), text(text), type(type)
{

}

void ErrMsg::getMessage()
{
	QMessageBox msgBox;
	switch (type) {
	case ERROR:
		msgBox.critical(nullptr, title, text);
		break;
	case WARNING:
		msgBox.warning(nullptr, title, text);
		break;
	case MESSAGE:
		msgBox.information(nullptr, title, text);
		break;
	}
}
