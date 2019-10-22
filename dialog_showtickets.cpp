#include "dialog_showtickets.h"
#include "ui_dialog_showtickets.h"

#include "config.h"
#include "ticket.h"
#include "train.h"
#include "wagon.h"

Dialog_ShowTickets::Dialog_ShowTickets(const Train &train, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_ShowTickets)
{
	mUi->setupUi(this);
	mUi->tableTickets->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	int row = 0;
	// Пробегаем по каждому вагону
	foreach (const Wagon &wagon, train.listWagons()) {
		// Пробегаем по каждому билету каждого вагона
		foreach (const Ticket &ticket, wagon.listTickets()) {
			// Заполняем таблицу
			QTableWidgetItem *item_id = new QTableWidgetItem(ticket.id());
			QTableWidgetItem *item_wagon = new QTableWidgetItem(QString::number(ticket.numberWagon()));
			QTableWidgetItem *item_type = new QTableWidgetItem(ticket.typeWagon());
			QTableWidgetItem *item_price = new QTableWidgetItem(QString::number(ticket.price()));
			QTableWidgetItem *item_dateTimeBuy =
					new QTableWidgetItem(ticket.dateTimeBuy().toString("dd.MM.yyyy hh:mm:ss"));

			mUi->tableTickets->insertRow(row); // Вставляем строку
			// Устанавливаем элементы таблицы в саму таблицу
			mUi->tableTickets->setItem(row, 0, item_id);
			mUi->tableTickets->setItem(row, 1, item_wagon);
			mUi->tableTickets->setItem(row, 2, item_type);
			mUi->tableTickets->setItem(row, 3, item_price);
			mUi->tableTickets->setItem(row, 4, item_dateTimeBuy);
			row++;
		}
	}
}

Dialog_ShowTickets::~Dialog_ShowTickets()
{
	delete mUi;
}
