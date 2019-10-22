#ifndef DIALOG_MENUADMIN_H
#define DIALOG_MENUADMIN_H

#include <QDialog>

namespace Ui {
class Dialog_MenuAdmin;
}

// Окно со всеми возможностями админа
class Dialog_MenuAdmin : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_MenuAdmin(QWidget *parent = nullptr);
	~Dialog_MenuAdmin();

private slots:
	// Управление учетными записями
	void on_buttonControlUsers_clicked();
	// Управление поездами
	void on_buttonControlRaces_clicked();

private:
	Ui::Dialog_MenuAdmin *mUi;
};

#endif // DIALOG_MENUADMIN_H
