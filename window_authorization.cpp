#include "window_authorization.h"
#include "ui_window_authorization.h"

#include "config.h"
#include "user.h"

#include <QFile>
#include <QDataStream>

Window_Authorization::Window_Authorization(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::Window_Authorization)
{
	mUi->setupUi(this);

	// Соединяем кнопку "Создать пользователя" с сигналом открытия окна регистрации.
	// Далее этот сигнал будет передавать главному окну, чтобы тот открыл окно регистрации.
	connect(mUi->buttonCreateUser, SIGNAL(clicked()),
			this, SIGNAL(openRegistration()));
}

Window_Authorization::~Window_Authorization()
{
	delete mUi;
}

//! Проверка на корректно введенные данные.
User *Window_Authorization::checkUser(const QString &login, const QString &password)
{
	QFile file(Config::fileUsers);
	// Если файл существует...
	if (file.exists())
	{
		// Создаем нулевой указатель.
		// Если пользователь с введенными данными не будет найден (или пароль не совпал),
		// данный указатель так и останется нулем.
		User *user = nullptr;

		// Если не удалось открыть файл для чтения...
		if (!file.open(QIODevice::ReadOnly)) {
			// Выводим ошибку.
			mUi->labelError->setText("Ошибка: открытие файла невозможно!");
			// Завершаем работу метода.
			return nullptr;
		}
		// Создаем поток для упрощенного считывания данных из файла.
		QDataStream ist(&file);
		// Пока не конец файла...
		while (!ist.atEnd()) {
			// Считываем данные.
			User buf_user;
			ist >> buf_user;
			// Если данные считанного пользователя совпали с введнными...
			if (buf_user.login() == login && buf_user.password() == password) {
				// Выделяем нулевому указателю память под объект тип User, присваиваем значение,
				// и возвращаем его.
				user = new User(buf_user);
				return user;
			}
		}

		// Если пользователь в цикле не был найден, возвращаем нулевой указатель.
		return user;
	}
	else
		return nullptr;
}

void Window_Authorization::on_buttonEntry_clicked()
{
	// Очищаем лейбл с ошибками.
	mUi->labelError->clear();

	// Считываем введенные данные в переменные.
	const QString login = mUi->login->text();
	const QString password = mUi->password->text();

	// Проверяем на корректность.
	if (login.isEmpty() || password.isEmpty()) {
		mUi->labelError->setText("Ошибка: заполните все поля!");
	}
	else if (login.size() < 3 || login.size() > 20) {
		mUi->labelError->setText("Ошибка: имя пользователя должено содержать от 3 до 20 символов!");
	}
	else if (password.size() < 6 || password.size() > 15) {
		mUi->labelError->setText("Ошибка: пароль должен содержать от 6 до 15 символов!");
	}
	else { // Если всё ОК...

		// Создаем указатель на объект типа User, который приравниваем возвращаемому типу метода
		// checkUser(), который сообщит нам, верно ли введны данные или нет.
		// При nullptr — данные введены неверно.
		// При не nullptr (был возвращен объект с данными) — данные введены корректно.
		User *user = checkUser(login, password);
		if (user == nullptr) {
			mUi->labelError->setText("Ошибка: введены неккоректные данные!");
		}
		else {
			// Посылаем сигнал об успешном входе.
			emit succesfulEntry(user);
			// После завершения сигнала будет открыто окно рабочего виджета.
		}
	}
}
