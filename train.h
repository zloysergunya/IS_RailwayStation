#ifndef TRAIN_H
#define TRAIN_H

#include "wagon.h"
#include <QString>
#include <QDate>
#include <QTime>

#include <QDataStream>
#include <QTextStream>

// Класс поезда
class Train
{
public:
	Train();
	Train(int number, const QDate &date, const QTime &time,
		  const QString &routeFrom, const QString &routeTo,
		  const QList<Wagon> &listWagons);

	// --- Методы получения и установки значений --- //
	void setData(int number, const QDate &date, const QTime &time,
				 const QString &routeFrom, const QString &routeTo,
				 const QList<Wagon> &listWagons);

	int number() const;
	void setNumber(int number);

	QDate date() const;
	void setDate(const QDate &date);

	QTime time() const;
	void setTime(const QTime &time);

	QString routeFrom() const;
	void setRouteFrom(const QString &routeFrom);

	QString routeTo() const;
	void setRouteTo(const QString &routeTo);

	QList<Wagon> listWagons() const;
	void setListWagons(const QList<Wagon> &listWagons);
	// --------------------------------------------- //

private:
	int m_number;
	QDate m_date;
	QTime m_time;
	QString m_routeFrom;
	QString m_routeTo;
	QList<Wagon> m_listWagons;
};

// Запись в файл
inline QDataStream &operator<< (QDataStream &ost, const Train &train)
{
	ost << train.number() << train.date() << train.time()
		<< train.routeFrom() << train.routeTo()
		<< train.listWagons().size() /*записываем количество вагонов*/;
	// Используя цикл записываем информацию о каждом вагоне в файл
	foreach (const Wagon &wagon, train.listWagons()) {
		ost << wagon;
	}
	return ost;
}

// Чтение из файла
inline QDataStream &operator>> (QDataStream &ist, Train &train)
{
	int number;
	QDate date;
	QTime time;
	QString routeFrom;
	QString routeTo;
	int countWagons;
	QList<Wagon> listWagons;

	ist >> number >> date >> time >> routeFrom >> routeTo
		>> countWagons /*считываем количество вагонов в данном поезде*/;
	// С помощью цикла считываем из файла все вагоны поочередно
	for (int i = 0; i < countWagons; i++) {
		Wagon wagon;
		ist >> wagon;
		listWagons.append(wagon);
	}
	train.setData(number, date, time, routeFrom, routeTo, listWagons);
	return ist;
}

inline QTextStream &operator<< (QTextStream &ost, const Train &train)
{
	ost << QString("НОМЕР ПОЕЗДА: ") << train.number()
		<< QString("\n*** ПОЕЗД ") << train.number() << " ***"
		<< QString("\nДата отпарвления: ") << train.date().toString("dd.MM.yyyy")
		<< QString("\nВремя отправления: ") << train.time().toString("hh:mm")
		<< QString("\nМаршрут: ") << train.routeFrom() << QString("—") << train.routeTo()
		<< QString("\nКоличество вагонов: ") << train.listWagons().size() << "\n";
	foreach (const Wagon &wagon, train.listWagons()) {
		ost << "\n" << wagon;
	}
	ost << QString("*** ПОЕЗД ") << train.number() << " ***";
	return ost;
}

#endif // TRAIN_H
