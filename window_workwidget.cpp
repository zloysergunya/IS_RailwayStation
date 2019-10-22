#include "window_workwidget.h"
#include "ui_window_workwidget.h"

#include "config.h"
#include "dialog_infouser.h"
#include "dialog_menuadmin.h"
#include "dialog_menucashier.h"

#include <QFile>
#include <QDataStream>

#include <QDebug>

Window_WorkWidget::Window_WorkWidget(User &user, QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::Window_WorkWidget),
	m_user(user)
{
	mUi->setupUi(this);
	// Устанавливаем ширину столбцов под размер содержимого.
	mUi->tableRaces->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

	// Выделяем память под окно с информацией о пользователе
	m_infoDialog = new Dialog_InfoUser(m_user, this);
	m_infoDialog->setWindowTitle(Config::nameApplication);

	// Если статус пользователя не равен Администратору, то прячем кнопку с открытием меню администратора
	if (m_user.status() != User::Admin)
		mUi->menuAdmin->hide();

	if (m_user.status() != User::Passenger)
		connect(mUi->tableRaces, SIGNAL(cellActivated(int, int)),
				this, SLOT(menuCashier(int, int)));

	// Загружаем список поездов и вагонов из файла в лист и таблицу
	loadTrains();
}

Window_WorkWidget::~Window_WorkWidget()
{
	// Очищаем память
	delete mUi;
	delete m_infoDialog;

	delete &m_user;
}

const QList<Train> &Window_WorkWidget::listTrains() const
{
	return m_listTrains;
}

// Слот добавления поезда
void Window_WorkWidget::addRace(Train train)
{
	// Добавляем в лист
	m_listTrains.append(train);

	// Сохраняем в файл.
	QFile file(Config::fileTrains);
	file.open(QIODevice::Append);
	QDataStream ost(&file);
	ost << train;

	// Добавляем в таблицу
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

// Слот редактирования поезда
void Window_WorkWidget::editRace(int row, Train train)
{
	// Изменяем данные в листе
	m_listTrains[row] = train;

	// #Перезапись_Данных
	// Подробно описана в файле Dialog_ControlUsers.cpp (#Перезапись_Данных)
	QFile read_file(Config::fileTrains); // Основной файл
	if (read_file.open(QIODevice::ReadOnly)) { // Основной файл открываем для чтения
		QFile write_file("buf_file_trains"); // Буфер-файл
		write_file.open(QIODevice::WriteOnly); // Буфер-файл для записи

		// Создаем потоки
		QDataStream read_ist(&read_file);
		QDataStream write_ist(&write_file);

		int countReads = 0;
		// Начинаем считывать
		while (!read_ist.atEnd()) {
			Train buf_train;
			read_ist >> buf_train;

			if (countReads++ == row) {
				buf_train = train;
			}

			write_ist << buf_train;
		}

		// Закрываем основной файл и удаляем его
		read_file.close();
		read_file.remove();
		// Закрываем буфер-файл и переименовываем его
		write_file.close();
		write_file.rename(Config::fileTrains);
	}

	// Изменяем таблицу
	mUi->tableRaces->item(row, 0)->setText(QString::number(train.number()));
	mUi->tableRaces->item(row, 1)->setText(train.date().toString("dd.MM.yyyy"));
	mUi->tableRaces->item(row, 2)->setText(train.time().toString("hh:mm"));
	mUi->tableRaces->item(row, 3)->setText(train.routeFrom());
	mUi->tableRaces->item(row, 4)->setText(train.routeTo());
	mUi->tableRaces->item(row, 5)->setText(QString::number(train.listWagons().size()));
}


void Window_WorkWidget::deleteRace(int row)
{
	// Удаляем рейс из листа
	m_listTrains.removeAt(row);

	// CTRL+F -> #Перезапись_Данных
	QFile read_file(Config::fileTrains); // Основной файл
	if (read_file.open(QIODevice::ReadOnly)) { // Основной файл открываем для чтения
		QFile write_file("buf_file_trains"); // Буфер-файл
		write_file.open(QIODevice::WriteOnly); // Буфер-файл для записи

		// Создаем потоки
		QDataStream read_ist(&read_file);
		QDataStream write_ist(&write_file);

		int countReads = 0;
		// Начинаем считывать
		while (!read_ist.atEnd()) {
			Train buf_train;
			read_ist >> buf_train;

			if (countReads++ != row) {
				write_ist << buf_train;
			}
		}

		// Закрываем основной файл и удаляем его
		read_file.close();
		read_file.remove();
		// Закрываем буфер-файл и переименовываем его
		write_file.close();
		write_file.rename(Config::fileTrains);
	}

	// Удаляем строку из таблицы
	mUi->tableRaces->removeRow(row);
}

void Window_WorkWidget::on_infoUser_clicked()
{
	// Если была нажата кнопка "Сменить пользователя" то окно вернет значение QDialog::Accepted
	// Иначе же QDialog::Rejected
	if (m_infoDialog->exec() == QDialog::Accepted) {
		// Посылаем сигнал об необходимости сменить пользователя
		emit changeUser();
	}
}

void Window_WorkWidget::on_menuAdmin_clicked()
{
	// Создаем окно с меню администратора и вызываем его
	Dialog_MenuAdmin dialog(this);
	dialog.setWindowTitle(Config::nameApplication);

	dialog.exec();
}

void Window_WorkWidget::menuCashier(int row, int /*column*/)
{
	// Получаем константную ссылку на поезд
	const Train &train = m_listTrains[row];
	// Создаем окно меню кассира
	Dialog_MenuCashier dialog(row, train, this);
	dialog.setWindowTitle(Config::nameApplication);

	// Соеденяем сигнал окна меню кассира редактирования рейса
	// со слотом редактирвоания рейса данного окна
	connect(&dialog, SIGNAL(editedRace(int, Train)),
			this, SLOT(editRace(int, Train)));

	dialog.exec();
}

// Загрузка поездов и вагонов
void Window_WorkWidget::loadTrains()
{
	// Очищаем на всякий случай лист
	m_listTrains.clear();

	// Считывание из файла
	QFile file(Config::fileTrains);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream ist(&file);

		while (!ist.atEnd()) {
			Train train;
			ist >> train;
			m_listTrains.append(train);
		}
	}

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

void Window_WorkWidget::on_lineFind_textChanged(const QString &text)
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
