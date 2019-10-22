#ifndef DIALOG_ADDWAGON_H
#define DIALOG_ADDWAGON_H

#include <QDialog>
class Wagon;

namespace Ui {
class Dialog_AddWagon;
}

// Окно добавления и редактирования вагонов
class Dialog_AddWagon : public QDialog
{
	Q_OBJECT

public:
	enum Type { Create /*создание вагона*/, Edit /*редактирвоание вагона*/ };

	// Конструктор получает: добавляемый/редактируемый вагон, лист вагонов для сравнения по номерам,
	// тип окна (добавление или редактирование)
	explicit Dialog_AddWagon(Wagon *wagon,
							 const QList<Wagon> &listWagons,
							 Type type,
							 QWidget *parent = nullptr);
	~Dialog_AddWagon();

private:
	Ui::Dialog_AddWagon *mUi;
	// Начальный номер вагона.
	// Необходим для проверки занятости номера.
	int m_startNumberWagon;
	Wagon *m_wagon;
	const QList<Wagon> &m_listWagons;

public slots:
	// Слот вызывающийся при нажатии кнопки "Ок"
	void accept();
};

#endif // DIALOG_ADDWAGON_H
