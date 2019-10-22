#ifndef DIALOG_SHOWTICKETS_H
#define DIALOG_SHOWTICKETS_H

#include <QDialog>
class Train;

namespace Ui {
class Dialog_ShowTickets;
}

// Окно просмотра списка купленных билетов на рейс
class Dialog_ShowTickets : public QDialog
{
	Q_OBJECT

public:
	// Конструктор получает: данные поезда
	explicit Dialog_ShowTickets(const Train &train, QWidget *parent = nullptr);
	~Dialog_ShowTickets();

private:
	Ui::Dialog_ShowTickets *mUi;
};

#endif // DIALOG_SHOWTICKETS_H
