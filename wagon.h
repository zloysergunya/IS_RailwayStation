#ifndef WAGON_H
#define WAGON_H

#include <QString>
#include <QDataStream>
#include <QTextStream>
#include "ticket.h"

class Wagon
{
public:
	Wagon();
	Wagon(int number, const QString type, int countPlace, QList<Ticket> listTickets, int price);

	// --- Методы получения и установки значений --- //
	void setData(int number, const QString type, int countPlace, QList<Ticket> listTicket, int price);

	int number() const;
	void setNumber(int value);

	QString type() const;
	void setType(const QString &type);

	int countPlace() const;
	void setCountPlace(int countPlace);

	int countBuyPlace() const;

	int price() const;
	void setPrice(int price);

	QList<Ticket> listTickets() const;
	void setListTickets(const QList<Ticket> &listTickets);
	// --------------------------------------------- //

	// Купить один билет
	void buyOneTicket(Ticket ticket);
	// Сдать один билет
	bool handOverOneTicket(const QString &id_ticket);

	// Количество свободных мест
	int countFreePlace() const;

private:
	int m_number;					// Номер вагона
	QString m_type;					// Тип вагона
	int m_countPlace;				// Количество мест
	int m_price;					// Цена
	QList<Ticket> m_listTickets;	// Лист билетов
};

inline QDataStream &operator<< (QDataStream &ost, const Wagon &wagon)
{
	ost << wagon.number() << wagon.type()
		<< wagon.countPlace() << wagon.price() << wagon.countBuyPlace();
	foreach (const Ticket &ticket, wagon.listTickets()) {
		ost << ticket;
	}
	return ost;
}

inline QDataStream &operator>> (QDataStream &ist, Wagon &wagon)
{
	QString type;
	int number, cPlace, cBuyPlace, price;
	ist >> number >> type >> cPlace >> price >> cBuyPlace;
	QList<Ticket> listTickets;
	for (int i = 0; i < cBuyPlace; i++) {
		Ticket ticket;
		ist >> ticket;
		listTickets.append(ticket);
	}

	wagon.setData(number, type, cPlace, listTickets, price);
	return ist;
}

inline QTextStream &operator<< (QTextStream &ost, const Wagon &wagon)
{
	ost << QString("НОМЕР ВАГОНА: ") << wagon.number() << QString("\n—————— НАЧАЛО ВАГОН ——————")
		<< QString("\nТип вагона: ") << wagon.type()
		<< QString("\nКоличество мест: ") << wagon.countPlace()
		<< QString("\nКоличество купленных билетов: ") << wagon.countBuyPlace()
		<< QString("\nЦена за билет: ") << wagon.price() << "\n\n"
		<< QString("Билеты:\n");
	foreach (const Ticket &ticket, wagon.listTickets()) {
		ost << "\n" << ticket;
	}
	ost << QString("—————— КОНЕЦ ВАГОН ——————\n");
	return ost;
}

#endif // WAGON_H
