#include "dialog_controlusers.h"
#include "ui_dialog_controlusers.h"

#include "config.h"
#include "dialog_adduser.h"

#include <QFile>
#include <QDataStream>
#include <QMessageBox>

#include <QDebug>

Dialog_ControlUsers::Dialog_ControlUsers(QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::Dialog_ControlUsers)
{
	mUi->setupUi(this);

	// Считываем данные о пользователях из файла и заносим их в таблицу
	QFile file(Config::fileUsers);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream ist(&file);

		int row = 0;
		while (!ist.atEnd()) {
			User user;
			ist >> user;
			m_listUsers.append(user);

			// Создаем элементы таблицы с нужным текстом
			QTableWidgetItem *item_login = new QTableWidgetItem(user.login());			// Логин
			QTableWidgetItem *item_status = new QTableWidgetItem(user.statusString());	// Статус

			mUi->tableUsers->insertRow(row); // Вставляем строку
			// Устанавливаем элементы таблицы в саму таблицу
			mUi->tableUsers->setItem(row, 0, item_login);
			mUi->tableUsers->setItem(row, 1, item_status);
			row++;
		}
	}
}

Dialog_ControlUsers::~Dialog_ControlUsers()
{
	delete mUi;
}

void Dialog_ControlUsers::slotUpAndDownStatus()
{
	// Иммется две кнопки для повышения и понижения статуса.
	// Чтобы не писать два по своей сути идентичных слота используем один общий.
	// Разница лишь в том, что будем проверять какая кнопка была нажата (повысить или понизить)
	// и в нужном случае выполнить необходимый метод. (upStatus() или downStatus()).
	// qobject_cast<> преобразует объект к нужному типу.
	// В нашем случае это QPushButton.
	// sender() — функция которая возвращает указатель на объект который вызвал этот слот.
	// По умолчанию sender() возвращает объект типа QObject, именно поэтому мы преобразуем его в QPushButton.
	QPushButton *btn = qobject_cast<QPushButton*>(sender());

	// Получаем выбранную строку
	int currentRow = mUi->tableUsers->currentRow();
	// Проверяем что пользователь действительно выбрал учетную запись из таблицы.
	// Если же не выбрал то currentRow будет равно -1
	if (currentRow != -1)
	{
		// Получаем логин из выбранной строки
		const QString login = mUi->tableUsers->item(currentRow, 0)->text();
		const QString status = mUi->tableUsers->item(currentRow, 1)->text();

		// Если выбранный пользователь является администратором
		// и это единственный администратор в системе, то его понизить невозможно
		if (status == "Администратор" && countAdmins() <= 1) {
			QMessageBox::warning(this, windowTitle(),
								 "Ошибка: не удалось повысить/понизить пользователя!");
			return;
		}

		// #Перезапись_Данных
		// Далее будет такая схема:
		// Открываем файл со всеми пользователями,
		// одновременно с этим создаем буфер-файл в который в дальнейшем будем переписывать данные
		// из основного файла. В конце просто удалим основной файл с пользователями,
		// а буфер-файл переименуем под название основного файла.
		// Таким образом происходит перезапись данных двоичного файла.
		QFile read_file(Config::fileUsers); // Основной файл
		if (read_file.open(QIODevice::ReadOnly)) { // Основной файл открываем для чтения
			QFile write_file("buf_file_users"); // Буфер-файл
			write_file.open(QIODevice::WriteOnly); // Буфер-файл для записи

			// Создаем потоки
			QDataStream read_ist(&read_file);
			QDataStream write_ist(&write_file);

			// Начинаем считывать
			while (!read_ist.atEnd()) {
				User user;
				read_ist >> user;

				// Если логин считанного пользователя совпал с логином редактируемого пользователя
				if (user.login() == login) {
					// Проверяем какая кнопка была нажата
					// Проверяем это с помозью имени объекта
					// Кнопка для повышения статуса имеет имя "buttonUpStatus"
					// Кнопка для понижения статуса имеет имя "buttonDownStatus"
					if (btn->objectName() == "buttonUpStatus") {
						// Проверка на "удалось ли повысить статус"
						if (!user.upStatus()) {
							QMessageBox::warning(this, windowTitle(),
												 "Ошибка: невозможно повысить статус!\n"
												 "Установлен максимальный уровень статуса.");
						}
					}
					else {
						// Проверка на "удалось ли понизить статус"
						if (!user.downStatus()) {
							QMessageBox::warning(this, windowTitle(),
												 "Ошибка: невозможно понизить статус!\n"
												 "Установлен минимальный уровень статуса.");
						}
					}

					// Изменяем статус в таблице
					mUi->tableUsers->item(currentRow, 1)->setText(user.statusString());
				}

				// Записываем пользователя в буфер-файл
				write_ist << user;
			}

			// Закрываем основной файл и удаляем его
			read_file.close();
			read_file.remove();
			// Закрываем буфер-файл и переименовываем его
			write_file.close();
			write_file.rename(Config::fileUsers);
		}
	}
	else {
		QMessageBox::warning(this, windowTitle(),
							 "Ошибка: не удалось повысить/понизить пользователя!");
	}
}

void Dialog_ControlUsers::on_buttonDeleteUser_clicked()
{
	int currentRow = mUi->tableUsers->currentRow();
	// Проверяем что пользователь действительно выбрал учетную запись из таблицы.
	// Если же не выбрал то currentRow будет равно -1
	if (currentRow != -1)
	{
		const QString login = mUi->tableUsers->item(currentRow, 0)->text();
		const QString status = mUi->tableUsers->item(currentRow, 1)->text();

		// Если выбранный пользователь является администратором
		// и это единственный администратор в системе, то его понизить невозможно
		if (status == "Администратор" && countAdmins() <= 1) {
			QMessageBox::warning(this, windowTitle(),
								 "Ошибка: не удалось повысить/понизить пользователя!");
			return;
		}

		// Удаляем строку из таблицы с выбранным пользователем
		mUi->tableUsers->removeRow(currentRow);

		// CTRL+F -> #Перезапись_Данных
		QFile read_file(Config::fileUsers);
		if (read_file.open(QIODevice::ReadOnly)) {
			QFile write_file("buf_file_users");
			write_file.open(QIODevice::WriteOnly);

			QDataStream read_ist(&read_file);
			QDataStream write_ist(&write_file);

			while (!read_ist.atEnd()) {
				User user;
				read_ist >> user;

				// Перезаписываем все данные кроме тех, когда логины совпадут.
				if (user.login() != login) {
					write_ist << user;
				}
			}

			read_file.close();
			read_file.remove();
			write_file.close();
			write_file.rename(Config::fileUsers);
		}
	}
	else {
		QMessageBox::warning(this, windowTitle(),
							 "Ошибка: не удалось удалить пользователя!");
	}
}

void Dialog_ControlUsers::on_buttonAddUser_clicked()
{
	// Создаем окно для добавления пользователя
	Dialog_AddUser dialog(this);
	dialog.setWindowTitle(windowTitle());

	// Вызываем открытие окна, и если окно было успешно закрыто (был добавлен пользователь)
	if (dialog.exec() == QDialog::Accepted) {
		// Передаем из окна данные о пользователе сюда
		const User &user = dialog.getUser();

		// Записываем данные в файл
		QFile file(Config::fileUsers);
		file.open(QIODevice::Append);
		QDataStream ost(&file);
		ost << user;

		// Добавляем данные о новом пользователе в таблицу
		QTableWidgetItem *item_login = new QTableWidgetItem(user.login());
		QTableWidgetItem *item_status = new QTableWidgetItem(user.statusString());

		int row = mUi->tableUsers->rowCount();
		mUi->tableUsers->insertRow(row);
		mUi->tableUsers->setItem(row, 0, item_login);
		mUi->tableUsers->setItem(row, 1, item_status);
	}
}

int Dialog_ControlUsers::countAdmins() const
{
	QFile file(Config::fileUsers);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream ist(&file);

		int count = 0;
		while (!ist.atEnd()) {
			User user;
			ist >> user;
			if (user.status() == User::Admin) count++;
		}
		return count;
	}
	else return 0;
}

void Dialog_ControlUsers::on_lineFind_textChanged(const QString &text)
{
	QStringList listStr = text.split(" ");
	listStr.removeAll(QString());

	for (int i = 0; i < m_listUsers.size(); i++) {
		mUi->tableUsers->showRow(i);
	}

	int row = 0;
	foreach (const User &user, m_listUsers) {
		int count = 0;
		foreach (const QString &str, listStr) {
			if (user.login().contains(str) ||
					user.statusString().contains(str)) {
				count++;
			}
		}
		if (count != listStr.size()) {
			mUi->tableUsers->hideRow(row);
		}
		row++;
	}
}
