#include "CreateUserDialog.h"
#include <QMessageBox>

CreateUserDialog::CreateUserDialog(QWidget *parent, http_connection *conn)
	: QDialog(parent)
{
	setlocale(LC_ALL, "Russian");
	this->setFixedSize(310, 310);
	ui.setupUi(this);

	QObject::connect(ui.btn_Save, &QPushButton::clicked, this, &CreateUserDialog::click_save);
	QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &CreateUserDialog::click_cancel);
}

CreateUserDialog::~CreateUserDialog()
{
}

void CreateUserDialog::click_save() {
	std::string Login = ui.le_Login->text().toStdString();
	std::string Password = ui.le_1Password->text().toStdString();
	std::string Password2 = ui.le_2Password->text().toStdString();
	uint Age = ui.le_Age->text().toUInt();

	if (Login == "") {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Введите логин"), QMessageBox::NoButton, this).exec();
		return;
	}
	std::set<std::string> All_Users = conn->getAllUsers();
	for (std::string User : All_Users)
		if (Login == User) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Пользователь с таким логином уже существует"), QMessageBox::NoButton, this).exec();
			return;
		}

	if (Password.size() < 8) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Пароль должен состоять минимум из 8 символов"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (Password != Password2) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Пароль не совпадает"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (Age > 100 || Age <= 3) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Возраст введен неверно"), QMessageBox::NoButton, this).exec();
		return;
	}

	try {
		conn->createUser(Login, Password, Age);
		this->accept();
	}
	catch (std::exception& error) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString(error.what()), QMessageBox::NoButton, this).exec();
	}
}

void CreateUserDialog::click_cancel() {
	this->reject();
}
