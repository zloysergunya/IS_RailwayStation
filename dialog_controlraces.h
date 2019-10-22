#ifndef DIALOG_CONTROLRACES_H
#define DIALOG_CONTROLRACES_H

#include <QDialog>
#include "train.h"

namespace Ui {
class Dialog_ControlRaces;
}

// Окно для управления поездами
class Dialog_ControlRaces : public QDialog
{
	Q_OBJECT

public:
	// Конструктор получает: ссылку на лист поездов из главного окна
	// (редактируя этот лист в этом классе, он также будет изменяться и в главном окне (Window_WorkWidget)).
	explicit Dialog_ControlRaces(const QList<Train> &listTrains, QWidget *parent = nullptr);
	~Dialog_ControlRaces();

private slots:
	// Нажатие на "добавить"
	void on_buttonAddRace_clicked();
	// Нажатие на "редактировать"
	void on_buttonEditRace_clicked();
	// Нажатие на "удалить"
	void on_buttonDeleteRace_clicked();

	void on_lineFind_textChanged(const QString &arg1);

private:
	Ui::Dialog_ControlRaces *mUi;
	const QList<Train> &m_listTrains;

signals:
	// Сигнал отсылающийся при добавлении нового поезда.
	// Посылает добавленный поезд.
	void addedRace(Train);
	// Сигнал отсылающийся при редактировании поезда
	// Посылает номер редактируемой строки в таблице и листе, и также новые данные о позде.
	void editedRace(int, Train);
	// Сигнал отсылающийся при удалении поезда.
	// Посылает номер удаляемой строки в таблице и листе.
	void deletedRace(int);
};

#endif // DIALOG_CONTROLRACES_H
