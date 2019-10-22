#ifndef DIALOG_INFOUSER_H
#define DIALOG_INFOUSER_H

#include <QDialog>
class User;

namespace Ui {
class Dialog_InfoUser;
}

// Окно с просмотром информации о пользователе
class Dialog_InfoUser : public QDialog
{
	Q_OBJECT

public:

	explicit Dialog_InfoUser(const User &user, QWidget *parent = nullptr);
	~Dialog_InfoUser();

private:
	Ui::Dialog_InfoUser *mUi;
};

#endif // DIALOG_INFOUSER_H
