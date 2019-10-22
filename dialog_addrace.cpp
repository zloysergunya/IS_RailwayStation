#include "dialog_addrace.h"
#include "ui_dialog_addrace.h"

#include "config.h"
#include "train.h"
#include "dialog_controlwagons.h"

Dialog_AddRace::Dialog_AddRace(Train *train, const QList<Train> &listTrains, Type type, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_AddRace),
	m_train(train),
	m_listTrains(listTrains)
{
	mUi->setupUi(this);

	m_startNumberTrain = train->number();

	// Проверка для чего используется окно.
	// Если "редактирование"
	if (type == Edit) {
		// Изменяем заголовок и устанавливаем нужные занчения в виджеты
		mUi->mainLabel->setText("Редактирование рейса");
		mUi->number->setValue(m_train->number());
		mUi->date->setDate(m_train->date());
		mUi->time->setTime(m_train->time());
		mUi->routeFrom->setText(m_train->routeFrom());
		mUi->routeTo->setText(m_train->routeTo());
	}
	else {
		// Иначе (добавление) просто меняем заголовок, виджеты оствляя пустыми
		mUi->mainLabel->setText("Добавление рейса");
	}

	// Выделяем память под окно с управлением вагонами
	m_dialogControlWagons = new Dialog_ControlWagons(train->listWagons(), this);
	// Устанавливаем номер поезда в окно управления вагонами (просто для уведомления пользователя)
	m_dialogControlWagons->setNumberTrain(train->number());
	m_dialogControlWagons->setWindowTitle(Config::nameApplication);
}

Dialog_AddRace::~Dialog_AddRace()
{
	delete mUi;

	delete m_dialogControlWagons;
}

void Dialog_AddRace::accept()
{
	// Очищаем лейбл с выводом ошибок
	mUi->labelError->clear();

	// Переносим значения из виджетов в переменные
	int number = mUi->number->value();
	QDate date = mUi->date->date();
	QTime time = mUi->time->time();
	QString routeFrom = mUi->routeFrom->text();
	QString routeTo = mUi->routeTo->text();

	// Проверки...
	if (routeFrom.isEmpty() || routeTo.isEmpty()) {
		mUi->labelError->setText("Ошибка: заполните все поля!");
	}
	else {
		// Если начальный номер поезда не совпадает с введенным, то проверяем на занятость данный номер
		if (m_startNumberTrain != number) {
			// Проверка на совпадение номеров добавляемого/редактируемого поезда с имеющимися поездами
			foreach (const Train &train, m_listTrains) {
				// Если номера поездов совпадают...
				if (train.number() == number) {
					// Выводим ошибку что такой номер поезда занят
					mUi->labelError->setText("Ошибка: данный номер поезда занят!");
					// Завершаем работу слота
					return;
				}
			}
		}

		// Иначе, если номер поезда ни один не совпал, устанавливаем данные в наш указатель
		m_train->setData(number, date, time, routeFrom, routeTo, m_dialogControlWagons->listWagons());
		// Сообщаем окну что оно может с успехом закрыться.
		QDialog::accept();
	}
}

// Если нажата кнопка "управление вагонами"
void Dialog_AddRace::on_buttonEditWagons_clicked()
{
	// Изменяем номер поезда на текущий (или измененный)
	m_dialogControlWagons->setNumberTrain(mUi->number->value());
	// Вызываем открытие окна.
	m_dialogControlWagons->exec();
}
