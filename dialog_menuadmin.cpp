
#include "dialog_menuadmin.h"
#include "ui_dialog_menuadmin.h"

#include "dialog_controlusers.h"
#include "dialog_controlraces.h"
#include "window_workwidget.h"

Dialog_MenuAdmin::Dialog_MenuAdmin(QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_MenuAdmin)
{
	mUi->setupUi(this);
}

Dialog_MenuAdmin::~Dialog_MenuAdmin()
{
	delete mUi;
}

void Dialog_MenuAdmin::on_buttonControlUsers_clicked()
{
	// Создаем окно управления учетными записями
	Dialog_ControlUsers dialog(this);
	dialog.setWindowTitle(windowTitle());

	dialog.exec();
}

void Dialog_MenuAdmin::on_buttonControlRaces_clicked()
{
	// Получаем лист из главного окна.
	// Использование ссылок обуславливается тем, чтобы не создавать копию листа,
	// потому что в этом нет необходимости. Мы можем использовать тот же самый лист.
	// А изменять его будет тот класс, который хранит его, т.е. Window_WorkWidget.
	// Т.е. окно управления рейсами не будет редактировать лист самостоятельно,
	// оно будет посылать сигналы на изменение данных, а изменять данные будет уже сам Window_WorkWidget.
	const QList<Train> &listTrains = qobject_cast<Window_WorkWidget*>(parent())->listTrains();
	// Создаем окно передавая в него лист.
	Dialog_ControlRaces dialog(listTrains, this);
	dialog.setWindowTitle(windowTitle());

	// Подключаем сигналы посылаемые из окна, такие как: добавление, редактирование и удаление рейса.
	connect(&dialog, SIGNAL(addedRace(Train)),
			parent(), SLOT(addRace(Train)));
	connect(&dialog, SIGNAL(editedRace(int, Train)),
			parent(), SLOT(editRace(int, Train)));
	connect(&dialog, SIGNAL(deletedRace(int)),
			parent(), SLOT(deleteRace(int)));

	// Вызываем окно
	dialog.exec();
}
