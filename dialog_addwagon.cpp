#include "dialog_addwagon.h"
#include "ui_dialog_addwagon.h"

#include "wagon.h"

Dialog_AddWagon::Dialog_AddWagon(Wagon *wagon, const QList<Wagon> &listWagons, Type type, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_AddWagon),
	m_wagon(wagon),
	m_listWagons(listWagons)
{
	mUi->setupUi(this);

	m_startNumberWagon = wagon->number();

	// Проверка для чего используется окно.
	// Если "редактирование"
	if (type == Edit) {
		// Изменяем заголовок и устанавливаем нужные занчения в виджеты
		mUi->mainLabel->setText("Редактирование вагона");
		mUi->number->setValue(wagon->number());
		mUi->type->setText(wagon->type());
		mUi->countPlace->setValue(wagon->countPlace());
		mUi->price->setValue(wagon->price());
	}
	else {
		// Иначе (добавление) просто меняем заголовок, виджеты оствляя пустыми
		mUi->mainLabel->setText("Добавление вагона");
	}
}

Dialog_AddWagon::~Dialog_AddWagon()
{
	delete mUi;
}

void Dialog_AddWagon::accept()
{
	// Очищаем лейбл с выводом ошибок
	mUi->labelError->clear();

	// Переносим значения из виджетов в переменные
	int number = mUi->number->value();
	QString type = mUi->type->text();
	int countPlace = mUi->countPlace->value();
	int price = mUi->price->value();

	// Проверки...
	if (type.isEmpty()) {
		mUi->labelError->setText("Ошибка: заполните все поля!");
	}
	else if (countPlace < m_wagon->countBuyPlace()) {
		mUi->labelError->setText("Ошибка: количество мест не может быть\nменьше уже купленных билетов!");
	}
	else {
		// Если начальный номер вагона не совпадает с введнным, то проверяем на занятость данного номера
		if (m_startNumberWagon != number) {
			// Проверка на совпадение номеров добавляемого/редактируемого вагона с имеющимися вагонами
			foreach (const Wagon &wagon, m_listWagons) {
				// Если номера вагонов совпадают...
				if (wagon.number() == number) {
					mUi->labelError->setText("Ошибка: введенный номер вагона занят!");
					return;
				}
			}
		}

		// Инче если все ок устанавливаем данные в редактируемый вагон
		m_wagon->setData(number, type, countPlace, m_wagon->listTickets(), price);
		QDialog::accept();
	}
}
