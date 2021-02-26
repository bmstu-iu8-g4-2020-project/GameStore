#pragma once

#include <QDialog>
#include "ui_UpdateBalanceDialog.h"
#include "http_connection.h"

class UpdateBalanceDialog : public QDialog
{
    Q_OBJECT

public:
    UpdateBalanceDialog(QWidget* parent, http_connection* conn);
    ~UpdateBalanceDialog();

public slots:
    int exec();

private slots:
    void click_AddMoney();
    void click_Cancel();

private:
    Ui::UpdateBalanceDialog ui;
    http_connection* conn;
};
