#pragma once

#include <string>
#include <filesystem>
#include <set>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QObject>


class http_connection : public QObject{
	Q_OBJECT
private:
	QByteArray data;
	QString server_ip_address;
	quint16 port;
	bool flag;

private slots:
	void processResponse();
	void readResponse();

public:
	http_connection();

	bool createUser(const std::string name, const std::string password, const uint8_t age);
	bool createGame(const std::string name, const std::string description, const uint8_t age_restriction, const double price);

	std::set<std::string> getAllGames();
	std::string getGameDescription(const std::string name);
	std::string getGameAgeRestriction(const std::string name);
	std::string getGamePrice(const std::string name);

	std::set<std::string> getAllUsers();
	std::string getUserPassword(const std::string name);
	std::string getUserAge(const std::string name);
	std::string getUserBalance(const std::string name);
	std::set<std::string> getUserGames(const std::string name);
	std::set<std::string> getUserFavorites(const std::string name);

	void updateGameName(const std::string name, const std::string new_name);
	void updateGameDescription(const std::string name, const std::string new_description);
	void updateGameAgeRestriction(const std::string name, const uint8_t new_age_restriction);
	void updateGamePrice(const std::string name, const double new_price);
	void removeGameFiles(const std::string name, const std::string directory_with_game);

	void updateUserName(const std::string name, const std::string new_name);
	void updateUserPassword(const std::string name, const std::string new_password);
	void updateUserAge(const std::string name, const uint8_t new_age);
	void updateUserBalance(const std::string name, const double new_balance);

	void addGameToUser(const std::string user_name, const std::string game_name);
	void addFavoriteGameToUser(const std::string user_name, const std::string game_name);
	void deleteFavoriteGameFromUser(const std::string user_name, const std::string game_name);
	void deleteGameFromUser(const std::string user_name, const std::string game_name);

	QString test(QString server_ip_address, quint16 port);
};