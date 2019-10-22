#include "window_registration.h"
#include "ui_window_registration.h"

#include "config.h"
#include "user.h"

#include <QFile>
#include <QDataStream>

Window_Registration::Window_Registration(TypeRegistration typeReg, QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::Window_Registration)
{
	mUi->setupUi(this);

	// Если регистрируется администратор, то мы прячем кнопку авторизации,
	// потому что регистрация администратора проходит только в том случае,
	// если в системе не найден ни один пользователь.
	if (typeReg == Admin) {
		mUi->mainLabel->setText("Регистрация администратора");
		mUi->buttonAuthorization->hide();
	}
	m_typeReg = typeReg;

	// Соединяем кнопку "Авторизация" с сигналом открытия окна авторизации.
	// Далее этот сигнал будет передавать главному окну, чтобы тот открыл окно авторизации.
	connect(mUi->buttonAuthorization, SIGNAL(clicked()),
			this, SIGNAL(openAuthorization()));
}

Window_Registration::~Window_Registration()
{
	delete mUi;
}

// Проверка на существование пользователя в системе.
// Вернет true - если существует; false - если не существует.
bool Window_Registration::isLoginExists(const QString &login)
{
	QFile file(Config::fileUsers);
	// Если файл существует
	if (file.exists()) {
		// Если не удалось открыть файл...
		if (!file.open(QIODevice::ReadOnly)) {
			// Сообщаем об ошибке.
			mUi->labelError->setText("Ошибка: чтение файла невозможно!");
			return false;
		}
		// Создаем поток.
		QDataStream ist(&file);

		// Пока не конец файла
		while (!ist.atEnd()) {
			// Считываем данные.
			User buf_user;
			ist >> buf_user;

			// Проверяем.
			if (buf_user.login() == login)
				return true;
		}

		return false;
	}
	else
		return false;
}

void Window_Registration::on_buttonCreate_clicked()
{
	// Очищаем лейбл с ошибками.
	mUi->labelError->clear();

	// Записываем введенные данные в переменные.
	const QString login = mUi->login->text();
	const QString password = mUi->password->text();
	const QString repeatPassword = mUi->repeatPassword->text();

	// Проверяем на корректность.
	if (login.isEmpty() || password.isEmpty() || repeatPassword.isEmpty()) {
		mUi->labelError->setText("Ошибка: заполните все поля!");
	}
	else if (isLoginExists(login)) {
		// Если во время выполнения метода произошла непредвиденная ошибка
		// (например, файл неудалось открыть), то сообщение об ошибке будет выведено в лейбл.
		// Если же ошибок не было, то лейбл останется пустым.
		if (mUi->labelError->text().isEmpty())
			mUi->labelError->setText("Ошибка: данное имя пользователя уже существует!");
	}
	else if (login.size() < 3 || login.size() > 20) {
		mUi->labelError->setText("Ошибка: имя пользователя должено содержать от 3 до 20 символов!");
	}
	else if (password.size() < 6 || password.size() > 15) {
		mUi->labelError->setText("Ошибка: пароль должен содержать от 6 до 15 символов!");
	}
	else if (password != repeatPassword) {
		mUi->labelError->setText("Ошибка: пароли не совпадают!");
	}
	else {
		// Создаем объект пользователя.
		User user(login,
				  password,
				  m_typeReg == Admin ? User::Admin : User::Passenger);

		QFile file(Config::fileUsers);
		// Открываем файл с флагом Append, который сообщает, что данные будут дописываться в файл.
		file.open(QIODevice::Append);
		QDataStream ost(&file);
		// Записываем пользователя.
		ost << user;

		// Посылаем сигнал об открытии окна Авторизации.
		emit openAuthorization();
	}
}
