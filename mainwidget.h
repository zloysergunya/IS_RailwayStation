#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
class User;

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = nullptr);
	~MainWidget();

private:
	QWidget *m_currentWidget;	// текущий отображаемый виджет
								// (авторизация, регистрация, окно с таблицами)

private slots:
	void slotOpenAuthorization();
	void slotOpenRegistration();
	void slotOpenWorkWidget(User*);
};
#endif // MAINWIDGET_H
