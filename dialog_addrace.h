#ifndef DIALOG_ADDRACE_H
#define DIALOG_ADDRACE_H

#include <QDialog>
class Train;
class Dialog_ControlWagons;

namespace Ui {
class Dialog_AddRace;
}

// Окно добавления и редактирования поездов
class Dialog_AddRace : public QDialog
{
	Q_OBJECT

public:
	enum Type { Create /*создание рейса*/, Edit /*редактирвоание рейса*/ };

	// Констркутор получает: указатель на добавляемый/редактируемый поезд, лист поездов для сравнения номеров,
	// тип окна (добавление или редактирование)
	explicit Dialog_AddRace(Train *train,
							const QList<Train> &listTrains,
							Type type,
							QWidget *parent = nullptr);
	~Dialog_AddRace();

private:
	Ui::Dialog_AddRace *mUi;
	// Начальный номер поезда при добавлении/редактировании
	// Необходим для проверки занятости номера.
	int m_startNumberTrain;
	// Указатель на переданный объект поезда
	Train *m_train;
	// Лист поездов для сравнения совпадений по номерам поездов.
	const QList<Train> &m_listTrains;

	// Окно для управления вагонами
	Dialog_ControlWagons *m_dialogControlWagons;

public slots:
	// Слоты вызывающийся когда пользователь нажмет кнопку "Ок"
	void accept();

private slots:
	// Нажатие на кнопку "Управление вагонами"
	void on_buttonEditWagons_clicked();
};

#endif // DIALOG_ADDRACE_H
