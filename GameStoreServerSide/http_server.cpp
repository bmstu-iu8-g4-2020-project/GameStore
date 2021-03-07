#include "http_server.h"
#include <QTcpSocket>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>


http_server::http_server(QObject* parent) : QTcpServer(parent), disabled(false) {
    quint16 port = 80;
    QHostAddress address("10.0.0.4");

    std::string line;
    std::ifstream settings("settings.txt");
    if (!settings.is_open())
        throw std::exception("file settings is not open");

    std::getline(settings, line);
    std::getline(settings, line);
    std::getline(settings, line);
    std::getline(settings, line);
    if (line.size() > 0)
        address = QString(line.c_str());
    std::getline(settings, line);
    if (line.size() > 0)
        port = QString(line.c_str()).toUInt();
    settings.close();

    listen(address, port);
}
void http_server::incomingConnection(qintptr socket) {
    if (disabled)
        return;
    QTcpSocket *socket_ptr = new QTcpSocket(this);
    connect(socket_ptr, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(socket_ptr, SIGNAL(disconnected()), this, SLOT(discardClient()));
    socket_ptr->setSocketDescriptor(socket);
    std::cout << "new connection: " << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << std::endl;
}
void http_server::pause() {
    disabled = true;
}
void http_server::resume() {
    disabled = false;
}
void http_server::readClient() {
    if (disabled)
        return;
    static bool is_connection = false;
    static const QStringList valid_commands({
        "createUser",
        "createGame",
        "addGameFiles",
        "presentGame",

        "getAllGames",
        "getGameDescription",
        "getGameAgeRestriction",
        "getGamePrice", 

        "getAllUsers",
        "getUserPassword",
        "getUserAge",
        "getUserBalance",
        "getUserGames",
        "getUserFavorites",

        "updateGameName",
        "updateGameDescription",
        "updateGameAgeRestriction",
        "updateGamePrice",
        "removeGameFiles",
        
        "updateUserName",
        "updateUserPassword",
        "updateUserAge",
        "updateUserBalance",

        "addGameToUser",
        "addFavoriteGameToUser",
        "deleteFavoriteGameFromUser",
        "deleteGameFromUser"
    });
    QTcpSocket* socket_ptr = static_cast<QTcpSocket*>(sender());

    std::string http_request(1024, '\0');
    qint64 read_bytes_count = socket_ptr->read(&http_request[0], 1024);
    if (read_bytes_count <= 0) {
        std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - can't read HTTP packet (" << read_bytes_count << "). Close connection" << std::endl;
        socket_ptr->close();
        return;
    }
    http_request = http_request.substr(0, read_bytes_count);
    std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - http request:" << std::endl << "---start---" << std::endl << http_request << std::endl << "---end---" << std::endl;
    if (!is_connection) {
        if (http_request.substr(0, 4) != "POST") {
            std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - invalid request" << std::endl;
            socket_ptr->close();
            return;
        }
        else {
            std::string::size_type pos_body = http_request.find("\r\n\r\n");
            if (pos_body == std::string::npos) {
                pos_body = http_request.find("\n\n");
                if (pos_body == std::string::npos) {
                    is_connection = true;
                    return;
                } else http_request = http_request.substr(pos_body + 2);
            } else http_request = http_request.substr(pos_body + 4);
            if (http_request.size() == 0) {
                is_connection = true;
                return;
            }

        }
    }
    QStringList tokens = QString(http_request.c_str()).split(QRegExp("[\r\n][\r\n]*"));

    if (tokens.size() < 2) {
        std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - invalid body. Close connection" << std::endl;
        is_connection = false;
        socket_ptr->close();
        return;
    }
    if (!valid_commands.contains(tokens[0])) {
        std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - invalid command. Close connection" << std::endl;
        is_connection = false;
        socket_ptr->close();
        return;
    }
    bool is_number = false;
    int number_of_parameters = 0;
    if (((number_of_parameters = tokens[1].toInt(&is_number)) < 0) || !is_number) {
        std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - invalid number of parameters. Close connection" << std::endl;
        is_connection = false;
        socket_ptr->close();
        return;
    }
    if (number_of_parameters != tokens.size() - 2) {
        std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - invalid parameters. Close connection" << std::endl;
        is_connection = false;
        socket_ptr->close();
        return;
    }

    QString command_result = start_command(tokens);
    std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - HTTP body:" << std::endl << "---start---" << std::endl << command_result.toStdString() << std::endl << "---end---" << std::endl;

    QTextStream response(socket_ptr);
    response.setAutoDetectUnicode(true);
    response << "HTTP/1.0 200 OK\r\nContent-Type: text/html; charset = \"utf-8\"\r\n\r\n" << command_result;
    
    socket_ptr->close();
    if (socket_ptr->state() == QTcpSocket::UnconnectedState) {
        std::cout << "closed" << std::endl;
    }
    is_connection = false;
}
void http_server::discardClient() {
    if (disabled)
        return;
    QTcpSocket* socket_ptr = static_cast<QTcpSocket*>(sender());
    std::cout << socket_ptr->peerAddress().toString().toStdString() << ":" << socket_ptr->peerPort() << " - closed" << std::endl;
    socket_ptr->deleteLater();
}

QString http_server::start_command(const QStringList& tokens) {

    if (tokens[0] == "createUser" && tokens[1] == "3") {
        try {
            conn.createUser(tokens[2].toStdString(), tokens[3].toStdString(), tokens[4].toInt());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "createGame" && tokens[1] == "4") {
        try {
            conn.createGame(tokens[2].toStdString(), tokens[3].toStdString(), tokens[4].toInt(), tokens[5].toDouble());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "addGameFiles" && tokens[1] == "1") {
        try {
            conn.addGameFiles(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getAllGames" && tokens[1] == "0") {
        try {
            std::set<std::string> games = conn.getAllGames();
            std::string s_games;
            for (std::string game : games)
                s_games += game + "|";
            
            return QString(s_games.c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getGameDescription" && tokens[1] == "1") {
        try {
            return QString(conn.getGameDescription(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getGameAgeRestriction" && tokens[1] == "1") {
        try {
            return QString(conn.getGameAgeRestriction(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getGamePrice" && tokens[1] == "1") {
        try {
            return QString(conn.getGamePrice(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getAllUsers" && tokens[1] == "0") {
        try {
            std::set<std::string> users = conn.getAllUsers();
            std::string s_users;
            for (std::string user : users)
                s_users += user + "|";

            return QString(s_users.c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getUserPassword" && tokens[1] == "1") {
        try {
            return QString(conn.getUserPassword(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getUserAge" && tokens[1] == "1") {
        try {
            return QString(conn.getUserAge(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getUserBalance" && tokens[1] == "1") {
        try {
            return QString(conn.getUserBalance(tokens[2].toStdString()).c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getUserGames" && tokens[1] == "1") {
        try {
            std::set<std::string> games = conn.getUserGames(tokens[2].toStdString());
            std::string s_games;
            for (std::string game : games)
                s_games += game + "|";

            return QString(s_games.c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "getUserFavorites" && tokens[1] == "1") {
        try {
            std::set<std::string> games = conn.getUserFavorites(tokens[2].toStdString());
            std::string s_games;
            for (std::string game : games)
                s_games += game + "|";

            return QString(s_games.c_str());
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateGameName" && tokens[1] == "2") {
        try {
            conn.updateGameName(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateGameDescription" && tokens[1] == "2") {
        try {
            conn.updateGameDescription(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateGameAgeRestriction" && tokens[1] == "2") {
        try {
            conn.updateGameAgeRestriction(tokens[2].toStdString(), tokens[3].toInt());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateGamePrice" && tokens[1] == "2") {
        try {
            conn.updateGamePrice(tokens[2].toStdString(), tokens[3].toDouble());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "removeGameFiles" && tokens[1] == "2") {
        try {
            conn.removeGameFiles(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateUserName" && tokens[1] == "2") {
        try {
            conn.updateUserName(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateUserPassword" && tokens[1] == "2") {
        try {
            conn.updateUserPassword(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateUserAge" && tokens[1] == "2") {
        try {
            conn.updateUserAge(tokens[2].toStdString(), tokens[3].toInt());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "updateUserBalance" && tokens[1] == "2") {
        try {
            conn.updateUserBalance(tokens[2].toStdString(), tokens[3].toDouble());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "addGameToUser" && tokens[1] == "2") {
        try {
            conn.addGameToUser(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "addFavoriteGameToUser" && tokens[1] == "2") {
        try {
            conn.addFavoriteGameToUser(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "deleteFavoriteGameFromUser" && tokens[1] == "2") {
        try {
            conn.deleteFavoriteGameFromUser(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    else if (tokens[0] == "deleteGameFromUser" && tokens[1] == "2") {
        try {
            conn.deleteGameFromUser(tokens[2].toStdString(), tokens[3].toStdString());
            return QString();
        }
        catch (std::exception& error) {
            return QString::fromWCharArray(L"Ошибка: ") + QString(error.what());
        }
    }
    return QString("command doesn't exist");
}