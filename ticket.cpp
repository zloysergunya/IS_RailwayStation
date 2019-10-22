#include "ticket.h"

Ticket::Ticket()
{

}

Ticket::Ticket(int number, int numTrain, int numWagon, const QString &typeWagon, const QDate &date, int price)
{
	setData(number, numTrain, numWagon, typeWagon, date, price);
}

void Ticket::setData(int number, int numTrain, int numWagon, const QString &typeWagon, const QDate &date, int price)
{
	m_number = number;
	m_numberTrain = numTrain;
	m_numberWagon = numWagon;
	m_typeWagon = typeWagon;
	m_date = date;
	m_price = price;
}

int Ticket::number() const
{
	return m_number;
}

void Ticket::setNumber(int number)
{
	m_number = number;
}

QString Ticket::id() const
{
	return m_id;
}

void Ticket::setId(const QString &id)
{
	m_id = id;
}

int Ticket::numberTrain() const
{
	return m_numberTrain;
}

void Ticket::setNumberTrain(int numberTrain)
{
	m_numberTrain = numberTrain;
}

int Ticket::numberWagon() const
{
	return m_numberWagon;
}

void Ticket::setNumberWagon(int numberWagon)
{
	m_numberWagon = numberWagon;
}

QString Ticket::typeWagon() const
{
	return m_typeWagon;
}

void Ticket::setTypeWagon(const QString &typeWagon)
{
	m_typeWagon = typeWagon;
}

QDate Ticket::date() const
{
	return m_date;
}

void Ticket::setDate(const QDate &date)
{
	m_date = date;
}

int Ticket::price() const
{
	return m_price;
}

void Ticket::setPrice(int price)
{
	m_price = price;
}

QDateTime Ticket::dateTimeBuy() const
{
    return m_dateTimeBuy;
}

void Ticket::setDateTimeBuy(const QDateTime &timeBuy)
{
    m_dateTimeBuy = timeBuy;
}
