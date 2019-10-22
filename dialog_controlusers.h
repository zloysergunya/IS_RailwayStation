#ifndef DIALOG_CONTROLUSERS_H
#define DIALOG_CONTROLUSERS_H

#include <QDialog>
#include "user.h"

namespace Ui {
class Dialog_ControlUsers;
}

// Окно для управления пользователями (повышать, понижать, добавлять, удалять)
class Dialog_ControlUsers : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_ControlUsers(QWidget *parent = nullptr);
	~Dialog_ControlUsers();

private slots:
	// Слот для повышения/понижения статуса
	void slotUpAndDownStatus();
	// Слот для удаления пользователя
	void on_buttonDeleteUser_clicked();
	// Слот для добавления пользователя
	void on_buttonAddUser_clicked();

	void on_lineFind_textChanged(const QString &arg1);

private:
	Ui::Dialog_ControlUsers *mUi;

	QList<User> m_listUsers;

	// Возвращает количество администраторов в системе
	int countAdmins() const;
};

#endif // DIALOG_CONTROLUSERS_H
