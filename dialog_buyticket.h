#ifndef DIALOG_BUYTICKET_H
#define DIALOG_BUYTICKET_H

#include <QDialog>
#include "wagon.h"
class Ticket;
class Train;

namespace Ui {
class Dialog_BuyTicket;
}

// Окно покупки билета (выбора вагона на который покупается билет)
class Dialog_BuyTicket : public QDialog
{
	Q_OBJECT

public:
	// Конструктор получает: константную ссылку на объект поезда (для экономии памяти)
	explicit Dialog_BuyTicket(const Train &train, QWidget *parent = nullptr);
	~Dialog_BuyTicket();

	// Возвращает измененный лист вагонов
	QList<Wagon> listWagons() const;

private:
	Ui::Dialog_BuyTicket *mUi;
	QList<Wagon> m_listWagons;
	const Train &m_train;

public slots:
	// Если пользователь нажал кнопку "Ок" то срабатывает этот слот.
	void accept();
};

#endif // DIALOG_BUYTICKET_H
