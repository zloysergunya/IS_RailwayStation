#ifndef WINDOW_WORKWIDGET_H
#define WINDOW_WORKWIDGET_H

#include <QWidget>
#include "train.h"
#include "user.h"
class Dialog_InfoUser;

namespace Ui {
class Window_WorkWidget;
}

class Window_WorkWidget : public QWidget
{
	Q_OBJECT

public:
	explicit Window_WorkWidget(User &user, QWidget *parent = nullptr);
	~Window_WorkWidget();

	const QList<Train> &listTrains() const;

public slots:
	void addRace(Train train);
	void editRace(int row, Train train);
	void deleteRace(int row);

private slots:
	// Открытие окна информации о пользователе
	void on_infoUser_clicked();
	// Открытие окна с меню администратора
	void on_menuAdmin_clicked();
	// Открытие меню кассира
	void menuCashier(int row, int column);

	void on_lineFind_textChanged(const QString &arg1);

signals:
	// Сигнал сообщающий об смене пользователя (закрытие текущего окна и открытие окна авторизации)
	void changeUser();

private:
	Ui::Window_WorkWidget *mUi;
	// Текущий пользователь.
	// Является ссылкой, для того чтобы не было необходимости очищать память после передачи данных.
	// Зачем очищать, если можно передать :)
	User &m_user;

	// Окно с информацией о пользователе
	// Необходимо хранить в самом классе для того, чтобы при смене пользователя
	// мы могли корректно удалить это окно.
	Dialog_InfoUser *m_infoDialog;

	QList<Train> m_listTrains;

	void loadTrains();
};

#endif // WINDOW_WORKWIDGET_H
