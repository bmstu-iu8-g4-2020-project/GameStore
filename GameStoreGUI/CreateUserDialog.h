#pragma once

#include <QDialog>
#include "ui_CreateUserDialog.h"
#include "http_connection.h"

class CreateUserDialog : public QDialog
{
	Q_OBJECT

public:
	CreateUserDialog(QWidget *parent, http_connection *conn);
	~CreateUserDialog();

private slots:
	void click_save();
	void click_cancel();

private:
	Ui::CreateUserDialog ui;
	http_connection *conn;
};
