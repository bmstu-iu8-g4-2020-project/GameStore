#include "CreateGameDialog.h"
#include <QMessageBox>
#include <QPushButton>

CreateGameDialog::CreateGameDialog(QWidget *parent, http_connection *conn)
	: QDialog(parent)
{
	setlocale(LC_ALL, "Russian");
	this->setFixedSize(550, 370);
	
	this->conn = conn;
	ui.setupUi(this);

	QObject::connect(ui.btn_Save, &QPushButton::clicked, this, &CreateGameDialog::click_save);
	QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &CreateGameDialog::click_cancel);
}

CreateGameDialog::~CreateGameDialog()
{
}

int CreateGameDialog::exec() {
	if (conn == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"База данных не найдена"), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}
	return QDialog::exec();
}

void CreateGameDialog::click_save() {
	std::string Name = ui.le_Name->text().toStdString();
	std::string Description = ui.le_Description->text().toStdString();
	uint Age_Restriction = ui.le_Age_Restriction->text().toUInt();
	bool is_Price;
	double Price = ui.le_Price->text().toDouble(&is_Price);
	if (Name == "") {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Имя игры введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	std::set<std::string> All_Games = conn->getAllGames();
	for (std::string Game : All_Games)
		if (Name == Game) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра с таким названием уже существует"), QMessageBox::NoButton, this).exec();
			return;
		}

	if (Description == "") {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Описание введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (Age_Restriction > 100 || Age_Restriction <= 3) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Возрастное ограничение введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (!is_Price) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Цена введена неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	try {
		if (!conn->createGame(Name, Description, static_cast<uint8_t>(Age_Restriction), Price))
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра не создана"), QMessageBox::NoButton, this).exec();
		else this->accept();
	}
	catch (std::exception &error) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString(error.what()), QMessageBox::NoButton, this).exec();
	}
}

void CreateGameDialog::click_cancel() {
	this->reject();
}
