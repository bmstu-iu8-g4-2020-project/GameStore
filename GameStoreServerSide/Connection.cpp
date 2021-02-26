#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <charconv>

//#include "sha256.h"
#include "Connection.h"


const std::string Connection::users_filename = "C:\\server\\DataBase\\usersData.txt";
const std::string Connection::games_filename = "C:\\server\\DataBase\\gamesData.txt";
const std::string Connection::directory_with_games = "C:\\server\\DataBase\\gameFiles";


// create ________
bool Connection::createUser(const std::string name, const std::string password, const uint8_t age) {
    std::ofstream work_with_file(users_filename, std::ios::app);
    work_with_file.write(name.c_str(), name.size());
    work_with_file << std::endl;
    //SHA256* cipher = new SHA256();
    //std::string encrypted_password = cipher->hash(password);
    //work_with_file.write(encrypted_password.c_str(), encrypted_password.size());
    //delete cipher;
    work_with_file << std::endl;
    work_with_file << static_cast<int>(age) << std::endl;
    work_with_file << 0 << std::endl; // строчка для баланса
    work_with_file << std::endl; // строчка для сохраненных игр
    work_with_file << std::endl; // строчка для избранных игр
    return true;
}
bool Connection::createGame(const std::string name, const std::string description, const uint8_t age_restriction, const double price) {
    std::ofstream work_with_file(games_filename, std::ios::app);
    work_with_file.write(name.c_str(), name.size());
    work_with_file << std::endl;
    work_with_file.write(description.c_str(), description.size());
    work_with_file << std::endl;
    work_with_file << static_cast<int>(age_restriction) << std::endl;
    work_with_file << price << std::endl;
    return true;
}
void Connection::addGameFiles(const std::string name, const std::filesystem::path directory_with_files) {
    std::filesystem::path save_directory = directory_with_games;
    save_directory /= name;
    save_directory /= directory_with_files;
    std::filesystem::copy_file(
    directory_with_files,
    (save_directory.filename())
    );
}

// get Game ______
std::set<std::string> Connection::getAllGames() {
	std::string find_g;
	std::set<std::string> games;
	uint64_t i = 0;
	
	std::ifstream work_with_file(games_filename, std::ios::in);
	while (std::getline(work_with_file, find_g)) {
		if (i % 4 == 0)
			games.emplace(find_g);
		i++;
	}

	return games;
}
std::string Connection::getGameDescription(const std::string  name)
{
    std::string find_n;
	uint64_t line_number = 0;
    std::ifstream work_with_file(games_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

	while (std::getline(work_with_file, find_n) ) {
		if (line_number % 4 == 0 && find_n == name && std::getline(work_with_file, find_n))
			return find_n;
		line_number++;
	}
	
    throw std::exception((std::string(__func__) + " :Игра не найдена").c_str());
}
std::string Connection::getGameAgeRestriction(const std::string name)
{
    std::string find_n;
	uint64_t line_number = 0;
    std::ifstream work_with_file(games_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n)) {
        if (line_number % 4 == 0 && find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n))
            return find_n;
		line_number++;
	}
    throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
}
std::string Connection::getGamePrice(const std::string name)
{
    std::string find_n;
	uint64_t line_number = 0;
    std::ifstream work_with_file(games_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n)) {
        if (line_number % 4 == 0 && find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n))
            return find_n;
		line_number++;
	}
    throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
}

// get User _____
std::set<std::string> Connection::getAllUsers() {
	std::string find_u;
	std::set<std::string> users;
	uint64_t i = 0;

	std::ifstream work_with_file(users_filename, std::ios::in);
	while (std::getline(work_with_file, find_u)) {
		if (i % 6 == 0)
			users.emplace(find_u);
		i++;
	}

	return users;
}
std::string Connection::getUserPassword(const std::string name)
{
	std::string find_n;
	std::ifstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	while (std::getline(work_with_file, find_n))
		if (find_n == name && std::getline(work_with_file, find_n))
			return find_n;
	throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
}
std::string Connection::getUserAge(const std::string name)
{
    std::string find_n;
    std::ifstream work_with_file(users_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n))
        if (find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n))
            return find_n;
    throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
}
std::string Connection::getUserBalance(const std::string name)
{
    std::string find_n;
    std::ifstream work_with_file(users_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n))
        if (find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n))
            return find_n;
    throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
}
std::set<std::string> Connection::getUserGames(const std::string name)  
{
    std::set<std::string> user_games;
    std::string find_n;
    std::ifstream work_with_file(users_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n))
        if (find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n)) {
			std::string game;
			std::istringstream all_games_stream(find_n);
			while (std::getline(all_games_stream, game, '|'))
				user_games.emplace(game);
			return user_games;
		}
    throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
	
	return std::set<std::string>();
}
std::set<std::string> Connection::getUserFavorites(const std::string name)
{
	std::set<std::string> user_f_games;
    std::string find_n;
    std::ifstream work_with_file(users_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    while (std::getline(work_with_file, find_n))
		if (find_n == name && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n) && std::getline(work_with_file, find_n)) {
			std::string game;
			std::istringstream all_games_stream(find_n);
			while (std::getline(all_games_stream, game, '|'))
				user_f_games.emplace(game);
			return user_f_games;
		}
    throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
}


// update Game ____
void Connection::updateGameName(const std::string name, const std::string new_name)
{
    std::fstream work_with_file(games_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    work_with_file.seekg(0, std::ios_base::end);
    size_t wwf_tellg = work_with_file.tellg();
    std::string game_data(wwf_tellg, '\0');

    work_with_file.seekg(0, std::ios_base::beg);
    work_with_file.read(&game_data[0], wwf_tellg);
    work_with_file.close();

    size_t pos_edit = game_data.find(name);
    if (pos_edit == std::string::npos)
        throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
    game_data.replace(pos_edit, name.size(), new_name);

    work_with_file.open(games_filename, std::ios::out);
    work_with_file.write(&game_data[0], strlen(&game_data[0]));
    work_with_file.close();
}
void Connection::updateGameDescription(const std::string name, const std::string new_description)
{
    std::fstream work_with_file(games_filename, std::ios::in);
    if (!work_with_file.is_open())
        throw std::exception("Файл не открыт");

    work_with_file.seekg(0, std::ios_base::end);
    size_t wwf_tellg = work_with_file.tellg();
    std::string game_data(wwf_tellg, '\0');

    work_with_file.seekg(0, std::ios_base::beg);
    work_with_file.read(&game_data[0], wwf_tellg);
    work_with_file.close();

    size_t pos_edit = game_data.find(name);
    if (pos_edit == std::string::npos)
        throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
    game_data.replace(pos_edit + name.size() + 1, getGameDescription(name).size(), new_description);

    work_with_file.open(games_filename, std::ios::out);
    work_with_file.write(&game_data[0], strlen(&game_data[0]));
    work_with_file.close();
}
void Connection::updateGameAgeRestriction(const std::string name, const uint8_t new_age_restriction)
{
	std::fstream work_with_file(games_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string game_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&game_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = game_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
	game_data.replace(pos_edit + name.size() + getGameDescription(name).size() + 2, getGameAgeRestriction(name).size(), std::to_string(new_age_restriction));
    
	work_with_file.open(games_filename, std::ios::out);
	work_with_file.write(&game_data[0], strlen(&game_data[0]));
	work_with_file.close();
}
void Connection::updateGamePrice(const std::string name, const double new_game_price)
{
	std::fstream work_with_file(games_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string game_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&game_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = game_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Игра не найдена").c_str());
	char buffer[10] = { '\0' };
	std::to_chars(buffer, buffer + 10, new_game_price, std::chars_format::fixed, 2);
	game_data.replace(pos_edit + name.size() + getGameDescription(name).size() + getGameAgeRestriction(name).size() + 3, getGamePrice(name).size(), buffer);

	work_with_file.open(games_filename, std::ios::out);
	work_with_file.write(&game_data[0], strlen(&game_data[0]));
	work_with_file.close();
}
void Connection::removeGameFiles(const std::string name, const std::string directory_with_game)
{	
}

// updete User _____
void Connection::updateUserName(const std::string name, const std::string new_name)
{
	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
	user_data.replace(pos_edit, name.size(), new_name);

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}
void Connection::updateUserPassword(const std::string name, const std::string new_password)
{
	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
	user_data.replace(pos_edit + name.size() + 1, getUserPassword(name).size(), new_password);

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}
void Connection::updateUserAge(const std::string name, const uint8_t new_age)
{
	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
	user_data.replace(pos_edit + name.size() + getUserPassword(name).size() + 2, getUserAge(name).size(), std::to_string(new_age));

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}
void Connection::updateUserBalance(const std::string name, const double new_balance)
{
	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());
	char buffer[10] = { '\0' };
	std::to_chars(buffer, buffer + 10, new_balance, std::chars_format::fixed, 2);
	user_data.replace(pos_edit + name.size() + getUserPassword(name).size() + getUserAge(name).size() + 3, getUserBalance(name).size(), buffer);

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}

// Games & User
void Connection::addGameToUser(const std::string user_name, const std::string game_name)
{
	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(user_name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());

	user_data.replace(pos_edit + user_name.size() + getUserPassword(user_name).size() + getUserAge(user_name).size() + getUserBalance(user_name).size() + 4, 0, game_name + "|");

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}
void Connection::addFavoriteGameToUser(const std::string user_name, const std::string game_name)
{
	std::set<std::string> games = getUserGames(user_name);

	std::string s_games;
	for (std::string game : games)
		s_games += game + "|";

	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(user_name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());

	user_data.replace(pos_edit + user_name.size() + getUserPassword(user_name).size() + getUserAge(user_name).size() + getUserBalance(user_name).size() + s_games.size() + 5, 0, game_name + "|");

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));

	work_with_file.close();
}		
void Connection::deleteGameFromUser(const std::string user_name, const std::string game_name)
{
	std::set<std::string> games = getUserGames(user_name);
	std::string s_games;
	for (std::string game : games) {
		if (game != game_name)
			s_games += game + "|";
	}

	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(user_name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());

	user_data.replace(pos_edit + user_name.size() + getUserPassword(user_name).size() + getUserAge(user_name).size() + getUserBalance(user_name).size() + 4, s_games.size() + game_name.size() + 1, s_games);

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}
void Connection::deleteFavoriteGameFromUser(const std::string user_name, const std::string game_name)
{
	std::set<std::string> games = getUserGames(user_name);
	std::string u_games;
	for (std::string game : games)
		u_games += game + "|";

	games = getUserFavorites(user_name);
	std::string s_games;
	for (std::string game : games) {
		if (game != game_name)
			s_games += game + "|";
	}

	std::fstream work_with_file(users_filename, std::ios::in);
	if (!work_with_file.is_open())
		throw std::exception("Файл не открыт");

	work_with_file.seekg(0, std::ios_base::end);
	size_t wwf_tellg = work_with_file.tellg();
	std::string user_data(wwf_tellg, '\0');

	work_with_file.seekg(0, std::ios_base::beg);
	work_with_file.read(&user_data[0], wwf_tellg);
	work_with_file.close();

	size_t pos_edit = user_data.find(user_name);
	if (pos_edit == std::string::npos)
		throw std::exception((std::string(__func__) + "Пользователь не найден").c_str());

	user_data.replace(pos_edit + user_name.size() + getUserPassword(user_name).size() + getUserAge(user_name).size() + getUserBalance(user_name).size() + u_games.size() + 5, s_games.size() + game_name.size() + 1, s_games);

	work_with_file.open(users_filename, std::ios::out);
	work_with_file.write(&user_data[0], strlen(&user_data[0]));
	work_with_file.close();
}