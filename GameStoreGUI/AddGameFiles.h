#pragma once

#include <QDialog>
#include "ui_AddGameFiles.h"
#include "http_connection.h"

class AddGameFiles : public QDialog
{
	Q_OBJECT

public:
	AddGameFiles(QWidget* parent, http_connection* conn);
	~AddGameFiles();

public slots:
	int exec();

private slots:
	void click_save();
	void click_cancel();
	void click_Browse();

private:
	Ui::AddGameFiles ui;
	http_connection* conn;
};