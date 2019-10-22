#include "mainwidget.h"

#include "config.h"
#include "window_registration.h"
#include "window_authorization.h"
#include "window_workwidget.h"

#include <QFile>
#include <QVBoxLayout>

#include <QDebug>
#include "user.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent),
	  m_currentWidget(nullptr)
{
	// Установка заголовка.
	setWindowTitle(Config::nameApplication);

	// Если файл с пользователями не существует...
	if (!QFile(Config::fileUsers).exists()) {
		// Создаем окно регистрации администратора при запуске.
		// Параметр 1 — определяет какой тип пользователя будет создаваться.
		m_currentWidget = new Window_Registration(Window_Registration::Admin, this);
		// Подключаем сигнал, посылаемый из класса Window_Registration,
		// который активирует открытие окна авторизации.
		connect(m_currentWidget, SIGNAL(openAuthorization()),
				this, SLOT(slotOpenAuthorization()));
	}
	else {
		// Иначе создаем окно авторизации.
		m_currentWidget = new Window_Authorization(this);
		// Соединяем сигнал, отправленный из класса Window_Authorization
		// при необходимости открыть окно регистрации, со слотом открытия окна регистрации.
		connect(m_currentWidget, SIGNAL(openRegistration()),
				this, SLOT(slotOpenRegistration()));
		// Соединяем сигнал, отправленный из класса Window_Authorization
		// при успешном входе в систему, со слотом открытия главного рабочего окна.
		connect(m_currentWidget, SIGNAL(succesfulEntry(User*)),
				this, SLOT(slotOpenWorkWidget(User*)));
	}

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0); // Делаем нулевые границы
	layout->addWidget(m_currentWidget);
	setLayout(layout);
}

MainWidget::~MainWidget()
{
	// Удаляем указатель на текущий рабочий виджет, для освобождения памяти.
	delete m_currentWidget;
}

void MainWidget::slotOpenAuthorization()
{
	// Удаляем текущее окно.
	delete m_currentWidget;

	// Создаем окно авторизации.
	m_currentWidget = new Window_Authorization(this);

	// Соеденяем все необходимые сигналы со слотами.
	connect(m_currentWidget, SIGNAL(openRegistration()),
			this, SLOT(slotOpenRegistration()));
	connect(m_currentWidget, SIGNAL(succesfulEntry(User*)),
			this, SLOT(slotOpenWorkWidget(User*)));

	// Добавляем новое созданное окно в Layout.
	layout()->addWidget(m_currentWidget);
}

void MainWidget::slotOpenRegistration()
{
	// Удаляем текущее окно.
	delete m_currentWidget;

	// Создаем окно регистрации пассажира.
	m_currentWidget = new Window_Registration(Window_Registration::Passenger, this);

	// Соеденяем все необходимые сигналы со слотами.
	connect(m_currentWidget, SIGNAL(openAuthorization()),
			this, SLOT(slotOpenAuthorization()));

	// Добавляем новое созданное окно в Layout.
	layout()->addWidget(m_currentWidget);
}

void MainWidget::slotOpenWorkWidget(User *user)
{
	// Удаляем текущее окно
	delete m_currentWidget;

	// Создаем окно рабочего виджета
	m_currentWidget = new Window_WorkWidget(*user, this);

	// Соеденяем все необходимые сигналы со слотами.
	connect(m_currentWidget, SIGNAL(changeUser()),
			this, SLOT(slotOpenAuthorization()));

	// Добавляем новое созданное окно в Layout.
	layout()->addWidget(m_currentWidget);
}

