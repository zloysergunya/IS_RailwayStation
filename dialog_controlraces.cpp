#include "dialog_controlraces.h"
#include "ui_dialog_controlraces.h"

#include "config.h"
#include "dialog_addrace.h"

#include <QMessageBox>
#include <QFile>
#include <QDataStream>

Dialog_ControlRaces::Dialog_ControlRaces(const QList<Train> &listTrains, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_ControlRaces),
	m_listTrains(listTrains)
{
	mUi->setupUi(this);
	// Устанавливаем ширину столбцов под размер содержимого.
	mUi->tableRaces->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	// Заполнение таблицы данными
	int row = 0;
	foreach (const Train &train, m_listTrains) {
		// Создаем элементы таблицы с нужным текстом
		QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(train.number()));
		QTableWidgetItem *item_date = new QTableWidgetItem(train.date().toString("dd.MM.yyyy"));
		QTableWidgetItem *item_time = new QTableWidgetItem(train.time().toString("hh:mm"));
		QTableWidgetItem *item_routeFrom = new QTableWidgetItem(train.routeFrom());
		QTableWidgetItem *item_routeTo = new QTableWidgetItem(train.routeTo());
		QTableWidgetItem *item_countWagons = new QTableWidgetItem(QString::number(train.listWagons().size()));

		mUi->tableRaces->insertRow(row); // Вставляем строку
		// Устанавливаем элементы таблицы в саму таблицу
		mUi->tableRaces->setItem(row, 0, item_number);
		mUi->tableRaces->setItem(row, 1, item_date);
		mUi->tableRaces->setItem(row, 2, item_time);
		mUi->tableRaces->setItem(row, 3, item_routeFrom);
		mUi->tableRaces->setItem(row, 4, item_routeTo);
		mUi->tableRaces->setItem(row, 5, item_countWagons);
		row++;
	}
}

Dialog_ControlRaces::~Dialog_ControlRaces()
{
	delete mUi;
}

void Dialog_ControlRaces::on_buttonAddRace_clicked()
{
	// Создаем объект поезда
	Train train;
	// Создаем окно в котором будут изменяться данные созданного объекта
	// Т.е. окно изменит данные поезда внутри себя, и после закрытия у нас остается этот же объект,
	// но с измененными данными.
	// Также передаем лист поездов для сравнения и передаем значения того что будет производится создание поезда.
	Dialog_AddRace dialog(&train, m_listTrains, Dialog_AddRace::Create, this);
	dialog.setWindowTitle(windowTitle());

	// Вызываем окно и проверяем как было закрыто окно.
	// Если была нажата кнопка "Ок", то окно завершится с флагом QDialog::Accepted
	if (dialog.exec() == QDialog::Accepted) {
		// Подаем сигнал, который подключен к главному классу (Window_WorkWidget),
		// на то что был добавлен новый поезд
		// (в главном классе будет выполнено добавление поезда в лист, сохранение в файл,
		// и новый поезд будет добавлен в таблицу.
		emit addedRace(train);

		// Обновляем таблицу в текущем окне.
		// Создаем элементы таблицы с нужным текстом
		QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(train.number()));
		QTableWidgetItem *item_date = new QTableWidgetItem(train.date().toString("dd.MM.yyyy"));
		QTableWidgetItem *item_time = new QTableWidgetItem(train.time().toString("hh:mm"));
		QTableWidgetItem *item_routeFrom = new QTableWidgetItem(train.routeFrom());
		QTableWidgetItem *item_routeTo = new QTableWidgetItem(train.routeTo());
		QTableWidgetItem *item_countWagons =
				new QTableWidgetItem(QString::number(train.listWagons().size()));

		int row = mUi->tableRaces->rowCount();
		mUi->tableRaces->insertRow(row); // Вставляем строку
		// Устанавливаем элементы таблицы в саму таблицу
		mUi->tableRaces->setItem(row, 0, item_number);
		mUi->tableRaces->setItem(row, 1, item_date);
		mUi->tableRaces->setItem(row, 2, item_time);
		mUi->tableRaces->setItem(row, 3, item_routeFrom);
		mUi->tableRaces->setItem(row, 4, item_routeTo);
		mUi->tableRaces->setItem(row, 5, item_countWagons);
	}
}

void Dialog_ControlRaces::on_buttonEditRace_clicked()
{
	// Проверяем какая страка была выбрана для редактирования
	int currentRow = mUi->tableRaces->currentRow();
	// Если строка не выбрана то currentRow будет равно -1
	if (currentRow != -1) {
		// Создаем копию выбранного рейса.
		Train train = m_listTrains[currentRow];
		// Создаем окно и передаем ему ссылку на объект поезда
		// Также устанавливаем значение что наш поезд будет "Редактироваться"
		Dialog_AddRace dialog(&train, m_listTrains, Dialog_AddRace::Edit, this);
		dialog.setWindowTitle(windowTitle());

		// Если окно было закрыто нажатием на кнопку "Ок"
		if (dialog.exec() == QDialog::Accepted) {
			// Посылаем сигнал в главный класс (Window_WorkWidget) в котором будет
			// выполнена перезапись в файл и обновлена таблица.
			// Передаем изменяемую строку и новые данные поезда.
			emit editedRace(currentRow, train);

			// Изменяем данные таблицы для текущего окна
			mUi->tableRaces->item(currentRow, 0)->setText(QString::number(train.number()));
			mUi->tableRaces->item(currentRow, 1)->setText(train.date().toString("dd.MM.yyyy"));
			mUi->tableRaces->item(currentRow, 2)->setText(train.time().toString("hh:mm"));
			mUi->tableRaces->item(currentRow, 3)->setText(train.routeFrom());
			mUi->tableRaces->item(currentRow, 4)->setText(train.routeTo());
			mUi->tableRaces->item(currentRow, 5)->setText(
						QString::number(train.listWagons().size()));
		}
	}
	else {
		// Если пользователь не выбрал рейс
		QMessageBox::warning(this, windowTitle(), "Ошибка: не выбран ни один рейс!");
	}
}

void Dialog_ControlRaces::on_buttonDeleteRace_clicked()
{
	int currentRow = mUi->tableRaces->currentRow();
	if (currentRow != -1) {
		// Посылаем сигнал в главный класс, который сообщает об необходимости удалить рейс из списка
		// Главный класс обновляет таблицу и происходит перезапись в файл.
		emit deletedRace(currentRow);

		// Удаляем строку из таблицы для текущего окна
		mUi->tableRaces->removeRow(currentRow);
	}
	else {
		QMessageBox::warning(this, windowTitle(), "Ошибка: не выбран ни один рейс!");
	}
}

void Dialog_ControlRaces::on_lineFind_textChanged(const QString &text)
{
	QStringList listStr = text.split(" ");
	listStr.removeAll(QString());

	for (int i = 0; i < m_listTrains.size(); i++) {
		mUi->tableRaces->showRow(i);
	}

	int row = 0;
	foreach (const Train &train, m_listTrains) {
		int count = 0;
		foreach (const QString &str, listStr) {
			if (train.date().toString("dd.MM.yyyy").contains(str) ||
					train.time().toString("hh:mm").contains(str) ||
					QString::number(train.number()).contains(str) ||
					train.routeFrom().contains(str) ||
					train.routeTo().contains(str)) {
				count++;
			}
		}
		if (count != listStr.size()) {
			mUi->tableRaces->hideRow(row);
		}
		row++;
	}
}
