#ifndef DIALOG_HANDOVERTICKET_H
#define DIALOG_HANDOVERTICKET_H

#include <QDialog>
#include "wagon.h"

namespace Ui {
class Dialog_HandOverTicket;
}

// Окно сдачи билета (выбор вагона для сдачи + выбор сдаваемого билета)
class Dialog_HandOverTicket : public QDialog
{
	Q_OBJECT

public:
	// Контркутор получает: лист вагонов
	explicit Dialog_HandOverTicket(const QList<Wagon> &listWagons, QWidget *parent = nullptr);
	~Dialog_HandOverTicket();

	// Возвращает измененный лист вагонов
	QList<Wagon> listWagons() const;

private:
	Ui::Dialog_HandOverTicket *mUi;
	QList<Wagon> m_listWagons;

public slots:
	void accept();
};

#endif // DIALOG_HANDOVERTICKET_H
