#include "dialog_menucashier.h"
#include "ui_dialog_menucashier.h"

#include "config.h"
#include "dialog_buyticket.h"
#include "dialog_handoverticket.h"
#include "dialog_showtickets.h"

#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QTextStream>

Dialog_MenuCashier::Dialog_MenuCashier(int row, const Train &train, QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_MenuCashier),
	m_train(train),
	m_rowTrain(row)
{
	mUi->setupUi(this);
	// Устанавливаем информацию в виджеты
	mUi->numberTrain->setText(QString::number(m_train.number()));
	mUi->countWagons->setText(QString::number(m_train.listWagons().size()));

	// Количество общего свободного места
	int countFreePlace = 0;
	foreach (const Wagon &wagon, m_train.listWagons()) {
		countFreePlace += wagon.countFreePlace();
	}

	// Если количество свободных мест равно 0
	if (countFreePlace == 0) {
		mUi->countFreePlace->setText("НЕТ МЕСТ");
		// Отключаем возможность купить билет (отключаем кнопку)
		mUi->buttonBuyTicket->setEnabled(false);
	}
	else {
		mUi->countFreePlace->setText(QString::number(countFreePlace));
	}
}

Dialog_MenuCashier::~Dialog_MenuCashier()
{
	delete mUi;
}

// Покупка билетов
void Dialog_MenuCashier::on_buttonBuyTicket_clicked()
{
	Dialog_BuyTicket dialog(m_train, this);
	dialog.setWindowTitle(windowTitle());

	if (dialog.exec() == QDialog::Accepted) {
		// Создаем новый объект поезда
		Train train = m_train;
		// Устанавливаем в него измененные данные о вагонах
		train.setListWagons(dialog.listWagons());

		// Посылаем сигнал об изменении данных
		emit editedRace(m_rowTrain, train);

		// Изменяем информацию об количестве свободных мест
		int countFreePlace = mUi->countFreePlace->text().toInt();
		countFreePlace--;
		if (countFreePlace == 0) {
			mUi->countFreePlace->setText("НЕТ МЕСТ");
			mUi->buttonBuyTicket->setEnabled(false);
		}
		else {
			mUi->countFreePlace->setText(QString::number(countFreePlace));
		}
	}
}

// Сдача билетов
void Dialog_MenuCashier::on_buttonHandOverTicket_clicked()
{
	Dialog_HandOverTicket dialog(m_train.listWagons(), this);
	dialog.setWindowTitle(windowTitle());

	if (dialog.exec() == QDialog::Accepted) {
		// Создаем новый объект поезда
		Train train = m_train;
		// Устанавливаем в него измененные данные о вагонах
		train.setListWagons(dialog.listWagons());

		// Посылаем сигнал об изменении данных
		emit editedRace(m_rowTrain, train);

		// Изменяем информацию об количестве свободных мест
		int countFreePlace = mUi->countFreePlace->text().toInt();
		countFreePlace++;
		if (countFreePlace == 0) {
			mUi->countFreePlace->setText("НЕТ МЕСТ");
			mUi->buttonBuyTicket->setEnabled(false);
		}
		else {
			mUi->countFreePlace->setText(QString::number(countFreePlace));
		}
	}
}

// Просмотр билетов
void Dialog_MenuCashier::on_buttonCheckTickets_clicked()
{
	Dialog_ShowTickets dialog(m_train, this);
	dialog.setWindowTitle(windowTitle());

	dialog.exec();
}

void Dialog_MenuCashier::on_buttonReferenceRace_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this,
													Config::nameApplication,
													QString(),
													"TXT (*.txt)");
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream ost(&file);
		ost.setCodec("UTF-8");

		ost << m_train;
	}
}
