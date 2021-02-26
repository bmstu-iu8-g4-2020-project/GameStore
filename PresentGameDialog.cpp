#include "PresentGameDialog.h"
#include "GameStoreGUI.h"
#include <QMessageBox>
#include <QPushButton>

PresentGameDialog::PresentGameDialog(QWidget* parent, http_connection* conn)
	: QDialog(parent)
{
	setlocale(LC_ALL, "Russian");
	this->setFixedSize(390, 320);

	this->conn = conn;
	ui.setupUi(this);

	QObject::connect(ui.btn_Present, &QPushButton::clicked, this, &PresentGameDialog::click_Present);
	QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &PresentGameDialog::click_Cancel);
}

PresentGameDialog::~PresentGameDialog()
{
}

int PresentGameDialog::exec() {
	if (conn == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"База данных не найдена"), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}

	for (const std::string& game : conn->getUserGames(static_cast<GameStoreGUI*>(this->parentWidget())->get_logged_user())) {
		ui.cb_Game->addItem(QString(game.c_str()));
	}
	for (const std::string& user : conn->getAllUsers()) {
		if (static_cast<GameStoreGUI*>(this->parentWidget())->get_logged_user() != user)
		ui.cb_User->addItem(QString(user.c_str()));
	}

	if (ui.cb_Game->currentText().toStdString().size() == 0) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"У Вас нет игр, что бы их подарить :("), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}
	if (ui.cb_User->currentText().toStdString().size() == 0) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Нет пользователей, кому бы можно было подарить игру :("), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}

	return QDialog::exec();
}

void PresentGameDialog::click_Present() {
	std::string Game = ui.cb_Game->currentText().toStdString();
	std::string User = ui.cb_User->currentText().toStdString();

	for (const std::string& game : conn->getUserGames(User)) {
		if (Game == game) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"У пользователя уже есть такая игра!"), QMessageBox::NoButton, this).exec();
			return;
		}
	}

	conn->addGameToUser(User, Game);
	conn->deleteGameFromUser(static_cast<GameStoreGUI*>(this->parentWidget())->get_logged_user(), Game);
	this->accept();
	return;
}

void PresentGameDialog::click_Cancel() {
	this->reject();
}
