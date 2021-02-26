#pragma once

#include <QDialog>
#include "ui_CreateGameDialog.h"
#include "http_connection.h"

class CreateGameDialog : public QDialog
{
	Q_OBJECT

public:
	CreateGameDialog(QWidget *parent, http_connection *conn);
	~CreateGameDialog();

public slots:
	int exec();

private slots:
	void click_save();
	void click_cancel();

private:
	Ui::CreateGameDialog ui;
	http_connection *conn;
};
