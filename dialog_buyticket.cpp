#include "dialog_buyticket.h"
#include "ui_dialog_buyticket.h"

#include "ticket.h"
#include "train.h"

#include <algorithm>
#include <QMessageBox>

Dialog_BuyTicket::Dialog_BuyTicket(const Train &train, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_BuyTicket),
	m_listWagons(train.listWagons()),
	m_train(train)
{
	mUi->setupUi(this);
	// Устанавливаем ширину столбцов под размер содержимого.
	mUi->tableWagons->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	// Заполняем таблицу вагонами
	int row = 0;
	foreach (const Wagon &wagon, m_listWagons) {
		// Создаем элементы таблицы с нужным текстом
		QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(wagon.number()));
		QTableWidgetItem *item_type = new QTableWidgetItem(wagon.type());
		QTableWidgetItem *item_countPlace = new QTableWidgetItem(QString::number(wagon.countPlace()));
		QTableWidgetItem *item_countFreePlace = new QTableWidgetItem(QString::number(wagon.countFreePlace()));
		QTableWidgetItem *item_price = new QTableWidgetItem(QString::number(wagon.price()));

		mUi->tableWagons->insertRow(row); // Вставляем строку
		// Устанавливаем элементы таблицы в саму таблицу
		mUi->tableWagons->setItem(row, 0, item_number);
		mUi->tableWagons->setItem(row, 1, item_type);
		mUi->tableWagons->setItem(row, 2, item_countPlace);
		mUi->tableWagons->setItem(row, 3, item_countFreePlace);
		mUi->tableWagons->setItem(row, 4, item_price);
		row++;
	}
}

Dialog_BuyTicket::~Dialog_BuyTicket()
{
	delete mUi;
}

QList<Wagon> Dialog_BuyTicket::listWagons() const
{
	return m_listWagons;
}

void Dialog_BuyTicket::accept()
{
	// Проверяем какой вагон был выбран
	int currentRow = mUi->tableWagons->currentRow();
	// Если вагон был не выбран значение currentRow будет равно -1
	if (currentRow != -1) {
		// Получаем ссылку на объект вагона из списка вагонов
		Wagon &wagon = m_listWagons[currentRow];

		// Если количество свободных мест на выбранный вагон равно 0
		if (wagon.countFreePlace() == 0) {
			mUi->labelError->setText("Ошибка: на данный вагон нет мест!");
		}
		else {
			// Создаем объект билета
			Ticket ticket;
			// Устанавливем данные
			ticket.setPrice(wagon.price());
			ticket.setTypeWagon(wagon.type());
			ticket.setNumberWagon(wagon.number());
			ticket.setDateTimeBuy(QDateTime::currentDateTime());
			ticket.setDate(m_train.date());
			ticket.setNumberTrain(m_train.number());
			// *** Поиск свободного номера билета *** //
			int freeNumber;
			for (freeNumber = 0; freeNumber < wagon.listTickets().size(); freeNumber++) {
				if (freeNumber != wagon.listTickets()[freeNumber].number())
					break;
			}
			// ************************************** //
			ticket.setNumber(freeNumber);
			// Создаем уникальный id на билет
			ticket.setId(QString::number(m_train.number()) + "_" +
						 m_train.date().toString("ddMMyy") + "_" +
						 QString::number(ticket.numberWagon()) + "_" +
						 QString::number(freeNumber));

			// Покупаем билет
			wagon.buyOneTicket(ticket);

			// Сортируем лист билетов по возратсанию (необходимо для корректного поиска совбодного номера билета)
			QList<Ticket> sortList = wagon.listTickets();
			std::sort(sortList.begin(), sortList.end());
			wagon.setListTickets(sortList);

			// Сообщаем об успешной покупке
			QMessageBox::information(this, windowTitle(),
									 "Билет успешно приобретен!"
									 "\n\nНомер билета: " + ticket.id() +
									 "\nЦена: " + QString::number(ticket.price()) +
									 "\nДата и время покупки: " + ticket.dateTimeBuy().toString("dd.MM.yyyy hh:mm:ss") +
									 "\n\nПоезд: " + QString::number(m_train.number()) +
									 "\nДата отправления: " + m_train.date().toString("dd.MM.yyyy") +
									 "\nВремя отправления: " + m_train.time().toString("hh:mm") +
									 "\n\nВагон номер: " + QString::number(wagon.number()) +
									 "\nТип вагона: " + wagon.type());

			QDialog::accept();
		}
	}
	else {
		mUi->labelError->setText("Ошибка: выберите вагон!");
	}
}
