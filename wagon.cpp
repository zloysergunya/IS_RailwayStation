#include "wagon.h"

Wagon::Wagon()
{
	m_price = 0;
	m_number = 0;
	m_countPlace = 0;
}

Wagon::Wagon(int number, const QString type, int countPlace, QList<Ticket> listTickets, int price)
{
	setData(number, type, countPlace, listTickets, price);
}

void Wagon::setData(int number, const QString type, int countPlace, QList<Ticket> listTicket, int price)
{
	m_number = number;
	m_type = type;
	m_countPlace = countPlace;
	m_listTickets = listTicket;
	m_price = price;
}

int Wagon::number() const
{
	return m_number;
}

void Wagon::setNumber(int value)
{
	m_number = value;
}

QString Wagon::type() const
{
	return m_type;
}

void Wagon::setType(const QString &type)
{
	m_type = type;
}

int Wagon::countPlace() const
{
	return m_countPlace;
}

void Wagon::setCountPlace(int countPlace)
{
	m_countPlace = countPlace;
}

int Wagon::countBuyPlace() const
{
	return m_listTickets.size();
}

int Wagon::price() const
{
	return m_price;
}

void Wagon::setPrice(int price)
{
	m_price = price;
}

QList<Ticket> Wagon::listTickets() const
{
	return m_listTickets;
}

void Wagon::setListTickets(const QList<Ticket> &listTickets)
{
	m_listTickets = listTickets;
}

void Wagon::buyOneTicket(Ticket ticket)
{
	m_listTickets.append(ticket);
}

bool Wagon::handOverOneTicket(const QString &id_ticket)
{
	int i = 0;
	foreach (const Ticket &ticket, m_listTickets) {
		if (ticket.id() == id_ticket) {
			m_listTickets.removeAt(i);
			return true;
		}
		i++;
	}
	return false;
}

int Wagon::countFreePlace() const
{
	return m_countPlace - m_listTickets.size();
}
