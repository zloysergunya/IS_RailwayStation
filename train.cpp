#include "train.h"

Train::Train()
{
	m_number = 0;
}

Train::Train(int number, const QDate &date, const QTime &time, const QString &routeFrom, const QString &routeTo, const QList<Wagon> &listWagons)
{
	setData(number, date, time, routeFrom, routeTo, listWagons);
}

void Train::setData(int number, const QDate &date, const QTime &time, const QString &routeFrom, const QString &routeTo, const QList<Wagon> &listWagons)
{
	m_number = number;
	m_date = date;
	m_time = time;
	m_routeFrom = routeFrom;
	m_routeTo = routeTo;
	m_listWagons = listWagons;
}

int Train::number() const
{
	return m_number;
}

void Train::setNumber(int number)
{
	m_number = number;
}

QDate Train::date() const
{
	return m_date;
}

void Train::setDate(const QDate &date)
{
	m_date = date;
}

QTime Train::time() const
{
	return m_time;
}

void Train::setTime(const QTime &time)
{
	m_time = time;
}

QString Train::routeFrom() const
{
	return m_routeFrom;
}

void Train::setRouteFrom(const QString &routeFrom)
{
	m_routeFrom = routeFrom;
}

QString Train::routeTo() const
{
	return m_routeTo;
}

void Train::setRouteTo(const QString &routeTo)
{
	m_routeTo = routeTo;
}

QList<Wagon> Train::listWagons() const
{
	return m_listWagons;
}

void Train::setListWagons(const QList<Wagon> &listWagons)
{
	m_listWagons = listWagons;
}
