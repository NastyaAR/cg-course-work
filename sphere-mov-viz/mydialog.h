#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

class MyDialog : public QDialog
{
public:
	MyDialog(QWidget *parent = nullptr) : QDialog(parent) {
		this->setWindowTitle("Новый источник света");
		QGridLayout *layout = new QGridLayout(this);

		QLabel label1 = QLabel("Направление	x:", this);
		layout->addWidget(&label1, 0, 0);
		lineEdits.push_back(new QLineEdit(this));
		layout->addWidget(lineEdits[0], 0, 1);

		QLabel label2 = QLabel("y:", this);
		layout->addWidget(&label2, 0, 2);
		lineEdits.push_back(new QLineEdit(this));
		layout->addWidget(lineEdits[1], 0, 3);

		QLabel label3 = QLabel("z:", this);
		layout->addWidget(&label3, 0, 4);
		lineEdits.push_back(new QLineEdit(this));
		layout->addWidget(lineEdits[2], 0, 5);

		QLabel label4 = QLabel("Мощность", this);
		layout->addWidget(&label4, 1, 0);
		lineEdits.push_back(new QLineEdit(this));
		layout->addWidget(lineEdits[3], 1, 1, 1, 5);

		QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
		layout->addWidget(buttonBox, 2, 0, 1, 6);

		// Соединяем сигналы кнопок с соответствующими слотами
		connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

		setLayout(layout);
	}

	~MyDialog() {
		for (auto lEdit : lineEdits)
			delete lEdit;
	}

	float getValue();
private:
	QVector<QLineEdit *> lineEdits;
};

#endif // MYDIALOG_H
