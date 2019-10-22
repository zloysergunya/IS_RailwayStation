#include "dialog_controlwagons.h"
#include "ui_dialog_controlwagons.h"

#include "wagon.h"
#include "dialog_addwagon.h"

#include <QMessageBox>

Dialog_ControlWagons::Dialog_ControlWagons(QList<Wagon> listWagons, QWidget *parent) :
	QDialog(parent), 
	mUi(new Ui::Dialog_ControlWagons),
	m_listWagons(listWagons)
{
	mUi->setupUi(this);
	mUi->tableWagons->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

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

Dialog_ControlWagons::~Dialog_ControlWagons()
{
	delete mUi;
}

void Dialog_ControlWagons::setNumberTrain(const int &number_train)
{
	// Устанавливаем номер поезда в лейбл для информирования пользователя о том для какого поезда
	// производится управление вагонами
	mUi->number_train->setText(QString::number(number_train));
}

void Dialog_ControlWagons::on_buttonAdd_clicked()
{
	// Создаем объект вагона
	Wagon wagon;
	// Создаем окно в которое по ссылке передаем объект вагона, лист вагонов
	// и флаг который сообщает что вагон будет добавляться.
	Dialog_AddWagon dialog(&wagon, m_listWagons, Dialog_AddWagon::Create, this);
	dialog.setWindowTitle(windowTitle());

	// Если окно было закрыто нажатием на кнопку "Ок"
	if (dialog.exec() == QDialog::Accepted) {
		// Добавляем новый вагон с спиок
		m_listWagons.append(wagon);

		// Изменяем таблицу
		QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(wagon.number()));
		QTableWidgetItem *item_type = new QTableWidgetItem(wagon.type());
		QTableWidgetItem *item_countPlace = new QTableWidgetItem(QString::number(wagon.countPlace()));
		QTableWidgetItem *item_countFreePlace = new QTableWidgetItem(QString::number(wagon.countFreePlace()));
		QTableWidgetItem *item_price = new QTableWidgetItem(QString::number(wagon.price()));

		int row = mUi->tableWagons->rowCount();
		mUi->tableWagons->insertRow(row); // Вставляем строку
		// Устанавливаем элементы таблицы в саму таблицу
		mUi->tableWagons->setItem(row, 0, item_number);
		mUi->tableWagons->setItem(row, 1, item_type);
		mUi->tableWagons->setItem(row, 2, item_countPlace);
		mUi->tableWagons->setItem(row, 3, item_countFreePlace);
		mUi->tableWagons->setItem(row, 4, item_price);
	}
}

void Dialog_ControlWagons::on_buttonEdit_clicked()
{
	int currentRow = mUi->tableWagons->currentRow();
	if (currentRow != -1) {
		// Создаем ссылку на объект листа
		// Теперь передав ссылку в другой класс (Dialog_AddWagon) мы можем изменить данные объект из вне этого метода
		Wagon &wagon = m_listWagons[currentRow];
		Dialog_AddWagon dialog(&wagon, m_listWagons, Dialog_AddWagon::Edit, this);
		dialog.setWindowTitle(windowTitle());

		// Если окно было закрыто нажатием кнопки "Ок"
		if (dialog.exec() == QDialog::Accepted) {
			// Изменяем данные в таблице этого окна (Dialog_ControlWagons)
			mUi->tableWagons->item(currentRow, 0)->setText(QString::number(wagon.number()));
			mUi->tableWagons->item(currentRow, 1)->setText(wagon.type());
			mUi->tableWagons->item(currentRow, 2)->setText(QString::number(wagon.countPlace()));
			mUi->tableWagons->item(currentRow, 3)->setText(QString::number(wagon.countFreePlace()));
			mUi->tableWagons->item(currentRow, 4)->setText(QString::number(wagon.price()));
		}
	}
	else {
		QMessageBox::warning(this, windowTitle(), "Ошибка: не выбран ни один вагон!");
	}
}

void Dialog_ControlWagons::on_buttonDelete_clicked()
{
	int currentRow = mUi->tableWagons->currentRow();
	if (currentRow != -1) {
		// Удаляем из списка
		m_listWagons.removeAt(currentRow);
		// Удаляем из таблицы
		mUi->tableWagons->removeRow(currentRow);
	}
	else {
		QMessageBox::warning(this, windowTitle(), "Ошибка: не выбран ни один вагон!");
	}
}

QList<Wagon> Dialog_ControlWagons::listWagons() const
{
	// Возвращаем созданный/отредактированный лист.
	return m_listWagons;
}

void Dialog_ControlWagons::on_lineFind_textChanged(const QString &text)
{
	QStringList listStr = text.split(" ");
	listStr.removeAll(QString());

	for (int i = 0; i < m_listWagons.size(); i++) {
		mUi->tableWagons->showRow(i);
	}

	int row = 0;
	foreach (const Wagon &wagon, m_listWagons) {
		int count = 0;
		foreach (const QString &str, listStr) {
			if (wagon.type().contains(str) ||
					QString::number(wagon.price()).contains(str) ||
					QString::number(wagon.number()).contains(str) ||
					QString::number(wagon.countPlace()).contains(str) ||
					QString::number(wagon.countFreePlace()).contains(str)) {
				count++;
			}
		}
		if (count != listStr.size()) {
			mUi->tableWagons->hideRow(row);
		}
		row++;
	}
}
