#include "EditGameDialog.h"
#include <QMessageBox>

EditGameDialog::EditGameDialog(QWidget *parent, http_connection *conn)
	: QDialog(parent)
{
	this->setFixedSize(540, 460);
	ui.setupUi(this);
	this->conn = conn;
	if (conn != nullptr)
		for (const std::string &game : conn->getAllGames())
			new QListWidgetItem(tr(game.c_str()), ui.lst_Games);
		

	QObject::connect(ui.btn_Save, &QPushButton::clicked, this, &EditGameDialog::click_save);
	QObject::connect(ui.btn_Cancel, &QPushButton::clicked, this, &EditGameDialog::click_cancel);
	QObject::connect(ui.lst_Games, &QListWidget::itemClicked, this, &EditGameDialog::game_selected);
}

EditGameDialog::~EditGameDialog()
{
}

int EditGameDialog::exec() {
	if (conn == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"База данных не найдена"), QMessageBox::NoButton, this).exec();
		return QDialog::Rejected;
	}
	return QDialog::exec();
}

void EditGameDialog::click_save() {

	std::string Name = ui.le_Name->text().toStdString();
	std::string Description = ui.le_Description->text().toStdString();
	uint Age_Restriction = ui.le_Age_Restriction->text().toUInt();
	bool is_Price;
	double Price = ui.le_Price->text().toDouble(&is_Price);

	std::set<std::string> All_Games = conn->getAllGames();
	for (std::string Game : All_Games)
		if (Name == Game && Name != s_game) {	
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Игра с таким названием уже существует"), QMessageBox::NoButton, this).exec();
			return;
		}

	if (Name == "") {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Имя игры введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (Description == "") {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Описание введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}
	if (Age_Restriction > 100 || Age_Restriction <= 3) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Возрастное ограничение введено неверно"), QMessageBox::NoButton, this).exec();
		return;
	}	
	if (!is_Price) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Цена введена неверно"), QMessageBox::NoButton, this).exec();
		return;
	}

	try {
		if (conn->getGameDescription(s_game) != Description)
			conn->updateGameDescription(s_game, Description);
		if (conn->getGameAgeRestriction(s_game) != ui.le_Age_Restriction->text().toStdString())
			conn->updateGameAgeRestriction(s_game, static_cast<uint8_t>(Age_Restriction));
		if (conn->getGamePrice(s_game) != ui.le_Price->text().toStdString())
			conn->updateGamePrice(s_game, Price);
		if (s_game != Name)
			conn->updateGameName(s_game, Name);

		this->accept();
	}
	catch (std::exception &error) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString(error.what()), QMessageBox::NoButton, this).exec();
	}
}

void EditGameDialog::click_cancel() {
	this->reject();
}

void EditGameDialog::game_selected(QListWidgetItem *item) {
	s_game = item->text().toStdString();
	
	ui.le_Name->setText(item->text());
	ui.le_Description->setText(QString(conn->getGameDescription(s_game).c_str()));
	ui.le_Age_Restriction->setText(QString(conn->getGameAgeRestriction(s_game).c_str()));
	ui.le_Price->setText(QString(conn->getGamePrice(s_game).c_str()));
}