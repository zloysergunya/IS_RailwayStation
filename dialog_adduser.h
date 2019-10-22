#ifndef DIALOG_ADDUSER_H
#define DIALOG_ADDUSER_H

#include <QDialog>
#include "user.h"

namespace Ui {
class Dialog_AddUser;
}

// Окно добавления нового пользователя
class Dialog_AddUser : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_AddUser(QWidget *parent = nullptr);
	~Dialog_AddUser();

	// Возвращает объект добавляемого пользователя
	const User &getUser() const;

private:
	Ui::Dialog_AddUser *mUi;
	// Хранит данные добавляемого пользователя
	User m_user;

public slots:
	// Данный слот перегружен от класса QDialog.
	// Вызов данного слота происходит когда пользователь нажмет кнопку "ОК"
	// В данном слоте записываются введенные данные в объект типа User
	void accept();
};

#endif // DIALOG_ADDUSER_H
