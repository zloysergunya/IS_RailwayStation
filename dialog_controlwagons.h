#ifndef DIALOG_CONTROLWAGONS_H
#define DIALOG_CONTROLWAGONS_H

#include <QDialog>
class Wagon;

namespace Ui {
class Dialog_ControlWagons;
}

class Dialog_ControlWagons : public QDialog
{
	Q_OBJECT

public:
	// Конструктор получает: лист вагонов от какого-либо поезда
	explicit Dialog_ControlWagons(QList<Wagon> listWagons,
								  QWidget *parent = nullptr);
	~Dialog_ControlWagons();

	// Установка в лейбл номера поезда, вагоны которого добавляются/редактируются/удаляются.
	// Нужно просто для уведомления пользователя.
	void setNumberTrain(const int &number_train);

	// Возвращает новый лист вагонов (отредактированный)
	QList<Wagon> listWagons() const;

private slots:
	// Нажатие на "добавить"
	void on_buttonAdd_clicked();
	// Нажатие на "редактировать"
	void on_buttonEdit_clicked();
	// Нажатие на "удалить"
	void on_buttonDelete_clicked();

	void on_lineFind_textChanged(const QString &arg1);

private:
	Ui::Dialog_ControlWagons *mUi;
	QList<Wagon> m_listWagons;
};

#endif // DIALOG_CONTROLWAGONS_H
