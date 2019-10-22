#ifndef DIALOG_MENUCASHIER_H
#define DIALOG_MENUCASHIER_H

#include <QDialog>
#include "train.h"

namespace Ui {
class Dialog_MenuCashier;
}

// Окно меню кассира (покупка/сдача билетов, получение каких-либо справок)
class Dialog_MenuCashier : public QDialog
{
	Q_OBJECT

public:
	// Конструктор получает: выбранную строку рейса в таблице главного окна (необходимо для редактирвоания данных),
	// константную ссылку на объект поезда (для экономии памяти).
	explicit Dialog_MenuCashier(int row, const Train &train, QWidget *parent = nullptr);
	~Dialog_MenuCashier();

private slots:
	// Покупка билета
	void on_buttonBuyTicket_clicked();
	// Сдача билета
	void on_buttonHandOverTicket_clicked();
	// Просмотр билетов
	void on_buttonCheckTickets_clicked();

	void on_buttonReferenceRace_clicked();

signals:
	// Сигнал посылающийся при необходимости имзенить данные рейса
	void editedRace(int, Train);

private:
	Ui::Dialog_MenuCashier *mUi;
	const Train &m_train;
	int m_rowTrain;
};

#endif // DIALOG_MENUCASHIER_H
