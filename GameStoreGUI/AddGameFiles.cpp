#include "AddGameFiles.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>


AddGameFiles::AddGameFiles(QWidget* parent, http_connection* conn)
	: QDialog(parent)
{
	setlocale(LC_ALL, "Russian");
	this->setFixedSize(420, 330);

	this->conn = conn;
	ui.setupUi(this);

	QObject::connect(ui.btn_Save, &QPushButton::clicked, this, &AddGameFiles::click_save);
	QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &AddGameFiles::click_cancel);
	QObject::connect(ui.btn_Browse, &QPushButton::clicked, this, &AddGameFiles::click_Browse);
}

AddGameFiles::~AddGameFiles()
{
}

int AddGameFiles::exec() {
	if (conn == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"База данных не найдена"), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}

	for (const std::string& game : conn->getAllGames()) {
		ui.cb_Game->addItem(QString(game.c_str()));
	}

	return QDialog::exec();
}

void AddGameFiles::click_save() {
	std::string Game = ui.cb_Game->currentText().toStdString();
	std::string GamePath = ui.le_GamePath->text().toStdString();

	if (Game.size() == 0) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Введите имя игры"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (GamePath.size() == 0) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Добавьте файлы игры"), QMessageBox::NoButton, this).exec();
		return;
	}
	std::set<std::string> All_Games = conn->getAllGames();
	for (std::string N_Game : All_Games) {
		if (Game == N_Game) {
			QDir GameDirectory(QString(http_connection::directory_with_games.c_str()));
			if (!GameDirectory.exists())
				GameDirectory.mkpath(QString(http_connection::directory_with_games.c_str()));
			QDir DirectoryWithGame(QString((http_connection::directory_with_games + "\\" + Game).c_str()));
			if (!DirectoryWithGame.exists())
				GameDirectory.mkpath(QString((http_connection::directory_with_games + "\\" + Game).c_str()));
			else {
				DirectoryWithGame.removeRecursively();
				GameDirectory.mkpath(QString((http_connection::directory_with_games + "\\" + Game).c_str()));
			}

			if (ui.le_GamePath->text().size() > 0) {
				if (!QFile::copy(ui.le_GamePath->text(), QString((http_connection::directory_with_games + "\\" + Game + "\\" + Game + ".exe").c_str())))
					QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Файл не найден."), QMessageBox::NoButton, this).exec();
				this->accept();
				return;
			}
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Выберите файл игры."), QMessageBox::NoButton, this).exec();
			return;
		}
	}
	QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра не найдена."), QMessageBox::NoButton, this).exec();
}

void AddGameFiles::click_cancel() {
	this->reject();
}

void AddGameFiles::click_Browse() {
	ui.le_GamePath->setText(QFileDialog::getOpenFileName(this, QString::fromWCharArray(L"Выбрать игру"), QString(), QString::fromWCharArray(L"Игры (*.exe)")));
}
