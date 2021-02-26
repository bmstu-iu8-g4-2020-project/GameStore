#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <charconv>
#include <QMessageBox>
#include <QTimer>
#include <QEventLoop>

#include "sha256.h"
#include "http_connection.h"


const std::string http_connection::users_filename = "C:\\Users\\fedor\\source\\repos\\Game Store GUI\\Game Store GUI\\DataBase\\usersData.txt";
const std::string http_connection::games_filename = "C:\\Users\\fedor\\source\\repos\\Game Store GUI\\Game Store GUI\\DataBase\\gamesData.txt";
const std::string http_connection::directory_with_games = "C:\\Users\\fedor\\source\\repos\\Game Store GUI\\Game Store GUI\\DataBase\\gameFiles";


http_connection::http_connection() {
	server_ip_address = "13.69.21.71";
	port = 80;
	data = "";
}

// create ________
bool http_connection::createUser(const std::string name, const std::string password, const uint8_t age)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "createUser\r\n3\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(password.c_str()) + "\r\n" + QByteArray(std::to_string(age).c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return false;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return false;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

			http_response->deleteLater();
			return true;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return false;
}
bool http_connection::createGame(const std::string name, const std::string description, const uint8_t age_restriction, const double price)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "createGame\r\n4\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(description.c_str()) + "\r\n" +
		QByteArray(std::to_string(age_restriction).c_str()) + "\r\n" + QByteArray::number(price, 'f', 2);
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return false;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return false;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return true;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return false;
}
void http_connection::addGameFiles(const std::string name, const std::filesystem::path directory_with_files)
{
	/*data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "getUserPassword\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return;*/
}

// get Game ______
std::set<std::string> http_connection::getAllGames()
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	std::string game;
	std::set<std::string> return_set;

	QNetworkAccessManager http_client;
	QNetworkReply* http_response;
	QByteArray req_data = "getAllGames\r\n0";
	http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return std::set<std::string>();
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return std::set<std::string>();
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200) {
			data = http_response->readAll();
			std::istringstream all_games_stream(data.toStdString());
			while (std::getline(all_games_stream, game, '|'))
				return_set.emplace(game);
		}

		http_response->deleteLater();
		return return_set;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return std::set<std::string>();
}
std::string http_connection::getGameDescription(const std::string  name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "getGameDescription\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}
std::string http_connection::getGameAgeRestriction(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "getGameAgeRestriction\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}
std::string http_connection::getGamePrice(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "getGamePrice\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}

// get User _____
std::set<std::string> http_connection::getAllUsers()
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);
	std::string user;
	std::set<std::string> return_set;

	QNetworkAccessManager http_client;
	QNetworkReply* http_response;
	QByteArray req_data = "getAllUsers\r\n0";
	http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return std::set<std::string>();
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return std::set<std::string>();
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200) {
			data = http_response->readAll();
			std::istringstream all_users_stream(data.toStdString());
			while (std::getline(all_users_stream, user, '|'))
				return_set.emplace(user);
		}

		http_response->deleteLater();
		return return_set;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return std::set<std::string>();
}
std::string http_connection::getUserPassword(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QByteArray req_data = "getUserPassword\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkAccessManager http_client;
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}
std::string http_connection::getUserAge(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "getUserAge\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}
std::string http_connection::getUserBalance(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "getUserBalance\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return data;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return data;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return data;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return data;
}
std::set<std::string> http_connection::getUserGames(const std::string name)  
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);
	std::string game;
	std::set<std::string> return_set;

	QNetworkAccessManager http_client;
	QByteArray req_data = "getUserGames\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return std::set<std::string>();
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return std::set<std::string>();
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200) {
			data = http_response->readAll();
			std::istringstream all_games_stream(data.toStdString());
			while (std::getline(all_games_stream, game, '|'))
				return_set.emplace(game);
		}

		http_response->deleteLater();
		return return_set;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return std::set<std::string>();
}
std::set<std::string> http_connection::getUserFavorites(const std::string name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);
	std::string user;
	std::set<std::string> return_set;

	QNetworkAccessManager http_client;
	QByteArray req_data = "getUserFavorites\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return std::set<std::string>();
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return std::set<std::string>();
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200) {
			data = http_response->readAll();
			std::istringstream all_games_stream(data.toStdString());
			while (std::getline(all_games_stream, user, '|'))
				return_set.emplace(user);
		}

		http_response->deleteLater();
		return return_set;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

	return std::set<std::string>();
}


// update Game ____
void http_connection::updateGameName(const std::string name, const std::string new_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateGameName\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(new_name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::updateGameDescription(const std::string name, const std::string new_description)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateGameDescription\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(new_description.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

}
void http_connection::updateGameAgeRestriction(const std::string name, const uint8_t new_age_restriction)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateGameAgeRestriction\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(std::to_string(new_age_restriction).c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

}
void http_connection::updateGamePrice(const std::string name, const double new_game_price)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateGamePrice\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(std::to_string(new_game_price).c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::removeGameFiles(const std::string name, const std::string directory_with_game)
{
	/*data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray data = "removeGameFiles\r\n1\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();*/
}

// updete User _____
void http_connection::updateUserName(const std::string name, const std::string new_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateUserName\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

}
void http_connection::updateUserPassword(const std::string name, const std::string new_password)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateUserPassword\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::updateUserAge(const std::string name, const uint8_t new_age)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateUserAge\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray(std::to_string(new_age).c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::updateUserBalance(const std::string name, const double new_balance)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "updateUserBalance\r\n2\r\n" + QByteArray(name.c_str()) + "\r\n" + QByteArray::number(new_balance, 'f', 2);
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}

// Games & User
void http_connection::addGameToUser(const std::string user_name, const std::string game_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "addGameToUser\r\n2\r\n" + QByteArray(user_name.c_str()) + "\r\n" + QByteArray(game_name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::addFavoriteGameToUser(const std::string user_name, const std::string game_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "addFavoriteGameToUser\r\n2\r\n" + QByteArray(user_name.c_str()) + "\r\n" + QByteArray(game_name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();

}		
void http_connection::deleteGameFromUser(const std::string user_name, const std::string game_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "deleteGameFromUser\r\n2\r\n" + QByteArray(user_name.c_str()) + "\r\n" + QByteArray(game_name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}
void http_connection::deleteFavoriteGameFromUser(const std::string user_name, const std::string game_name)
{
	data.clear();

	QTimer tim;
	tim.setInterval(10000);
	tim.setSingleShot(true);

	QNetworkAccessManager http_client;
	QByteArray req_data = "deleteFavoriteGameFromUser\r\n2\r\n" + QByteArray(user_name.c_str()) + "\r\n" + QByteArray(game_name.c_str());
	QNetworkReply* http_response = http_client.post(QNetworkRequest(QUrl("http://" + server_ip_address + ":" + QString::number(port) + "/")), req_data);
	if (http_response == nullptr) {
		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), QString::fromWCharArray(L"Ошибка подключения"), QMessageBox::NoButton).exec();
		return;
	}

	QEventLoop loop;
	connect(&tim, &QTimer::timeout, &loop, &QEventLoop::quit);
	connect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);

	tim.start();
	loop.exec();
	if (tim.isActive()) {
		tim.stop();
		if (http_response->error() != QNetworkReply::NoError) {
			QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();

			http_response->deleteLater();
			return;
		}
		int http_status = http_response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (http_status == 200)
			data = http_response->readAll();

		http_response->deleteLater();
		return;
	}
	disconnect(http_response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	http_response->abort();

	http_response->deleteLater();
}

void http_connection::processResponse() {
	QNetworkAccessManager http_client;
	QNetworkReply* http_response;
	if (http_response->error()) {

		QMessageBox(QMessageBox::Warning, QString::fromWCharArray(L"Ошибка"), http_response->errorString(), QMessageBox::NoButton).exec();
		http_response->deleteLater();
		http_response = nullptr;
		return;
	}
	data += http_response->readAll();


	http_response->deleteLater();
	http_response = nullptr;
}
void http_connection::readResponse() {
	QNetworkAccessManager http_client;
	QNetworkReply* http_response;
	data += http_response->readAll();
}