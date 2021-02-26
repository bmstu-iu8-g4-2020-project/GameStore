#include "UpdateBalanceDialog.h"
#include "GameStoreGUI.h"
#include <QMessageBox>
#include <QPushButton>

UpdateBalanceDialog::UpdateBalanceDialog(QWidget* parent, http_connection* conn)
    : QDialog(parent)
{
    setlocale(LC_ALL, "Russian");
    this->setFixedSize(400, 260);

    this->conn = conn;
    ui.setupUi(this);

    QObject::connect(ui.btn_AddMoney, &QPushButton::clicked, this, &UpdateBalanceDialog::click_AddMoney);
    QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &UpdateBalanceDialog::click_Cancel);
}

UpdateBalanceDialog::~UpdateBalanceDialog()
{
}

int UpdateBalanceDialog::exec() {
    if (conn == nullptr) {
        QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"База данных не найдена"), QMessageBox::NoButton, this).exec();
        return QDialog::Rejected;
    }
    return QDialog::exec();
}

void UpdateBalanceDialog::click_AddMoney() {
    std::string User_Name = static_cast<GameStoreGUI*>(this->parentWidget())->get_logged_user();
    bool is_Money;
    double Add_Money = ui.le_AddMoney->text().toDouble(&is_Money);

    if (!is_Money || Add_Money <= 0) {
        QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Сумма введена неверно"), QMessageBox::NoButton, this).exec();
        return;
    }

    try {
        conn->updateUserBalance(User_Name, atof(conn->getUserBalance(User_Name).c_str()) + Add_Money);
        this->accept();
    }
    catch (std::exception& error) {
        QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString(error.what()), QMessageBox::NoButton, this).exec();
    }
}

void UpdateBalanceDialog::click_Cancel() {
    this->reject();
}