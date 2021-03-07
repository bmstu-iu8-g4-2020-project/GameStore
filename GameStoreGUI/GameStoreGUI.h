#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GameStoreGUI.h"
#include "http_connection.h"

class GameStoreGUI : public QMainWindow
{
    Q_OBJECT

public:
    GameStoreGUI(QWidget *parent = Q_NULLPTR);
	std::string get_logged_user();

private slots:
	void login();
	void click_show_favorite();
	void click_show_games();
	void click_show_store();

	void click_create_user();
	void click_create_game();
	void click_update_game();

	void game_selected(QListWidgetItem* item);
	void click_open();
	void click_buy();
	void click_add_to_favorites();
	void click_delete_from_favorites();
	void click_Present();
	void click_add_money();

private:
    Ui::GameStoreGUIClass ui;
	http_connection conn;
	std::string s_game;
	std::string user;
};
