#pragma once

#include <QDialog>
#include "ui_EditGameDialog.h"
#include "http_connection.h"

class EditGameDialog : public QDialog
{
	Q_OBJECT

public:
	EditGameDialog(QWidget *parent, http_connection *conn);
	~EditGameDialog();

public slots:
	int exec();

private slots:
	void click_save();
	void click_cancel();
	void game_selected(QListWidgetItem *item);

private:
	Ui::EditGameDialog ui;
	http_connection *conn;
	std::string s_game;
};
