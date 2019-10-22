#include "dialog_handoverticket.h"
#include "ui_dialog_handoverticket.h"

#include <QInputDialog>

#include <QDebug>

Dialog_HandOverTicket::Dialog_HandOverTicket(const QList<Wagon> &listWagons, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_HandOverTicket),
	m_listWagons(listWagons)
{
	mUi->setupUi(this);
	mUi->tableWagons->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	// Заполняем таблицу списком вагонов
	int row = 0;
	foreach (const Wagon &wagon, m_listWagons) {
		// Создаем элементы таблицы с нужным текстом
		QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(wagon.number()));
		QTableWidgetItem *item_type = new QTableWidgetItem(wagon.type());
		QTableWidgetItem *item_countBuyPlace = new QTableWidgetItem(QString::number(wagon.countBuyPlace()));
		QTableWidgetItem *item_price = new QTableWidgetItem(QString::number(wagon.price()));

		mUi->tableWagons->insertRow(row); // Вставляем строку
		// Устанавливаем элементы таблицы в саму таблицу
		mUi->tableWagons->setItem(row, 0, item_number);
		mUi->tableWagons->setItem(row, 1, item_type);
		mUi->tableWagons->setItem(row, 2, item_countBuyPlace);
		mUi->tableWagons->setItem(row, 3, item_price);
		row++;
	}
}

Dialog_HandOverTicket::~Dialog_HandOverTicket()
{
	delete mUi;
}

QList<Wagon> Dialog_HandOverTicket::listWagons() const
{
	return m_listWagons;
}

void Dialog_HandOverTicket::accept()
{
	// Получаем выбранную строку
	int currentRow = mUi->tableWagons->currentRow();
	if (currentRow != -1) {
		// Получаем ссылку на объект выбранного вагона
		Wagon &wagon = m_listWagons[currentRow];

		// Если количество купленных билетов равно 0
		if (wagon.countBuyPlace() == 0) {
			mUi->labelError->setText("Ошибка: на данный вагон не куплен ни один билет!");
		}
		else {
			// Создаем дилоговое окно из стандартной библеотеки Qt (QInputDialog)
			// Это простое окно с вводом текста, либо с выбором строки из списка.
			// В нашем случае используется список билетов.
			QInputDialog dialog(this);
			// Устанавливаем опцию что окно будет использовать список, а не ввод текста (по умолчанию)
			dialog.setOption(QInputDialog::InputDialogOption::UseListViewForComboBoxItems);
			// Устанавливаем заголовк
			dialog.setLabelText("Выберите номер билета для сдачи");
			// Создаем лист из QString объектов
			QStringList listId;
			// Заполняем его идентификационными данными каждого билета
			foreach (const Ticket &ticket, wagon.listTickets()) {
				listId.append(ticket.id());
			}
			// Устанавливаем лист в окно
			dialog.setComboBoxItems(listId);
			// Изменяем заголовок
			dialog.setWindowTitle(windowTitle());
			// Изменяем размер под "желательный размер окна"
			dialog.resize(dialog.sizeHint());
			// Открываем окно и проверяем что пользователь нажал кнопку "Ок"
			if (dialog.exec() == QInputDialog::Accepted) {
				// Сдаем билет по выбранному идентификационному номеру
				wagon.handOverOneTicket(dialog.textValue());
				QDialog::accept();
			}
		}
	}
	else {
		mUi->labelError->setText("Ошибка: выберите вагон!");
	}
}
