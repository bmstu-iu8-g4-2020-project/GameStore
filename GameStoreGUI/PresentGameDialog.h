#pragma once

#include <QDialog>
#include "ui_PresentGameDialog.h"
#include "http_connection.h"

class PresentGameDialog : public QDialog
{
	Q_OBJECT

public:
	PresentGameDialog(QWidget* parent, http_connection* conn);
	~PresentGameDialog();

public slots:
	int exec();

private slots:
	void click_Present();
	void click_Cancel();

private:
	Ui::PresentGameDialog ui;
	http_connection* conn;
};
