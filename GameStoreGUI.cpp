#include "GameStoreGUI.h"
#include "CreateGameDialog.h"
#include "CreateUserDialog.h"
#include "EditGameDialog.h"
#include "PresentGameDialog.h"
#include "UpdateBalanceDialog.h"
#include "AddGameFiles.h"
#include "sha256.h"
#include <cstdlib>
#include <QObject>
#include <QListWidgetItem>
#include <QMenu>
#include <QMenuBar>
#include <iostream>
#include <QMessageBox>
#include <charconv>
#include <QProcess>
#include <QPushButton>

GameStoreGUI::GameStoreGUI(QWidget *parent)
    : QMainWindow(parent)
{
	setlocale(LC_ALL, "Russian");

    ui.setupUi(this);
	ui.menuBar->setVisible(false);
	ui.lbl_UserName->setVisible(false);
	ui.lbl_Balance->setVisible(false);
	ui.lst_Games->setVisible(false);
	ui.btn_Buy->setVisible(false);
	ui.btn_AddToFavorites->setVisible(false);
	ui.btn_DeleteFromFavorites->setVisible(false);
	this->setFixedSize(1280, 720);

	user = "";

	QList<QAction*> menus = ui.menuBar->actions();
	for (QAction* elem : menus[0]->menu()->actions()) {
		wstring a = elem->text().toStdWString();
		if (elem->text() == L"Избранное")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_show_favorite);
		else if (elem->text() == L"Игры")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_show_games);
		else if (elem->text() == L"Магазин")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_show_store);
		else if (elem->text() == L"Подарить игру")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_Present);
		else if (elem->text() == L"Пополнить баланс")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_add_money);
	}
	for (QAction* elem : menus[1]->menu()->actions()) {
		wstring a = elem->text().toStdWString();
		if (elem->text() == L"Изменить игру")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_update_game);
		else if (elem->text() == L"Добавить игру")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_create_game);
		else if (elem->text() == L"Добавить файлы игры")
			QObject::connect(elem, &QAction::triggered, this, &GameStoreGUI::click_add_files);
	}

	QObject::connect(ui.btn_Login, &QPushButton::clicked, this, &GameStoreGUI::login);
	QObject::connect(ui.btn_Registration, &QPushButton::clicked, this, &GameStoreGUI::click_create_user);

	QObject::connect(ui.lst_Games, &QListWidget::itemClicked, this, &GameStoreGUI::game_selected);
	QObject::connect(ui.btn_Buy, &QPushButton::clicked, this, &GameStoreGUI::click_buy);
	QObject::connect(ui.btn_AddToFavorites, &QPushButton::clicked, this, &GameStoreGUI::click_add_to_favorites);
	QObject::connect(ui.btn_DeleteFromFavorites, &QPushButton::clicked, this, &GameStoreGUI::click_delete_from_favorites);
}

void GameStoreGUI::login() {

	//conn.updateGameDescription("Death Stranding", "the new game");
	
	SHA256* cipher = new SHA256();
	std::string encrypted_password = cipher->hash(ui.le_Password->text().toStdString());
	delete cipher;
	if (conn.getUserPassword(ui.le_UserName->text().toStdString()) == encrypted_password) {
		ui.le_Password->setVisible(false);
		ui.le_UserName->setVisible(false);
		ui.btn_Login->setVisible(false);
		ui.btn_Registration->setVisible(false);

		ui.menuBar->setVisible(true);
		ui.lbl_UserName->setVisible(true);
		ui.lbl_Balance->setVisible(true);
		ui.lst_Games->setVisible(true);

		ui.lbl_UserName->setText(ui.le_UserName->text());
		user = ui.lbl_UserName->text().toStdString();
		ui.lbl_Balance->setText(QString(conn.getUserBalance(ui.le_UserName->text().toStdString()).c_str()));
		
		
		for (const std::string &game : conn.getUserGames(ui.le_UserName->text().toStdString())) {
			new QListWidgetItem(tr(game.c_str()), ui.lst_Games);
		}

	}
}

void GameStoreGUI::click_show_favorite() {
	ui.lst_Games->clear();

	ui.btn_Buy->setVisible(false);
	ui.btn_AddToFavorites->setVisible(false);
	ui.btn_DeleteFromFavorites->setVisible(false);

	for (const std::string &game : conn.getUserFavorites(ui.le_UserName->text().toStdString())) {
		new QListWidgetItem(tr(game.c_str()), ui.lst_Games);
	}
}
void GameStoreGUI::click_show_games() {
	ui.lst_Games->clear();

	ui.btn_Buy->setVisible(false);
	ui.btn_AddToFavorites->setVisible(false);
	ui.btn_DeleteFromFavorites->setVisible(false);

	for (const std::string &game : conn.getUserGames(ui.le_UserName->text().toStdString())) {
		new QListWidgetItem(tr(game.c_str()), ui.lst_Games);
	}
}
void GameStoreGUI::click_show_store() {
	ui.lst_Games->clear();

	ui.btn_Buy->setVisible(false);
	ui.btn_AddToFavorites->setVisible(false);
	ui.btn_DeleteFromFavorites->setVisible(false);

	for (const std::string &game : conn.getAllGames()) {
		new QListWidgetItem(tr(game.c_str()), ui.lst_Games);
	}
}


void GameStoreGUI::click_update_game() {
	EditGameDialog win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}
}
void GameStoreGUI::click_create_game() {
	CreateGameDialog win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}

}
void GameStoreGUI::click_create_user() {
	CreateUserDialog win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}
}
void GameStoreGUI::click_add_files() {
	AddGameFiles win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}
}
void GameStoreGUI::click_Present() {
	PresentGameDialog win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}
}
void GameStoreGUI::click_add_money() {
	UpdateBalanceDialog win(this, &conn);
	if (win.exec() == QDialog::Accepted) {}
	ui.lbl_Balance->setText(QString(conn.getUserBalance(ui.le_UserName->text().toStdString()).c_str()));
}


void GameStoreGUI::game_selected(QListWidgetItem* item) {
	ui.btn_Buy->setVisible(true);
	ui.btn_AddToFavorites->setVisible(true);
	ui.btn_DeleteFromFavorites->setVisible(true);

	s_game = item->text().toStdString();
}
void GameStoreGUI::click_open() {
	QProcess GameProc;

	std::set<std::string> All_Games = conn.getUserGames(ui.lbl_UserName->text().toStdString());
	for (std::string Game : All_Games)
		if (s_game == Game) {
			GameProc.start(QString((http_connection::directory_with_games + "\\" + s_game + "\\" + s_game + ".exe").c_str()), QStringList());
			return;
		}
	QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Сначала купите игру!"), QMessageBox::NoButton, this).exec();
}
void GameStoreGUI::click_buy() {
	std::set<std::string> All_Games = conn.getUserGames(ui.lbl_UserName->text().toStdString());
	for (std::string Game : All_Games)
		if (s_game == Game) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра уже куплена!"), QMessageBox::NoButton, this).exec();
			return;
		}

	double Balance = atof(conn.getUserBalance(ui.lbl_UserName->text().toStdString()).c_str());
	double Price = atof(conn.getGamePrice(s_game).c_str());
	if (Balance < Price) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"У Вас недостаточно средств :("), QMessageBox::NoButton, this).exec();
		return;
	}
	
	if (Balance < 0.001)
		Balance = 0.0;
	else Balance -= Price;

	conn.updateUserBalance(ui.lbl_UserName->text().toStdString(), Balance);
	char buffer[10] = { '\0' };
	std::to_chars(buffer, buffer + 10, Balance, chars_format::fixed, 2);
	ui.lbl_Balance->setText(QString(buffer));
	conn.addGameToUser(ui.lbl_UserName->text().toStdString(), s_game);
}
void GameStoreGUI::click_add_to_favorites() {
	std::set<std::string> F_Games = conn.getUserFavorites(ui.lbl_UserName->text().toStdString());
	for (std::string F_Game : F_Games)
		if (s_game == F_Game) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра уже добавлена в избранное!"), QMessageBox::NoButton, this).exec();
			return;
		}
	conn.addFavoriteGameToUser(ui.lbl_UserName->text().toStdString(), s_game);
}
void GameStoreGUI::click_delete_from_favorites() {
	std::set<std::string> F_Games = conn.getUserFavorites(ui.lbl_UserName->text().toStdString());
	for (std::string F_Game : F_Games)
		if (s_game == F_Game) {
			conn.deleteFavoriteGameFromUser(ui.lbl_UserName->text().toStdString(), s_game);
			return;
		}
	QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игры уже нет в избранном!"), QMessageBox::NoButton, this).exec();
}


std::string GameStoreGUI::get_logged_user() {
	return user;
}