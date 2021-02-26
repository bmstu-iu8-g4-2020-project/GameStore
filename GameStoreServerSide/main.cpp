#include "http_server.h"
#include <QtCore/QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    http_server *serv = new http_server(80, QHostAddress("10.0.0.4"), &a);

    if (!serv->isListening()) {
        std::cout << "Server is not listen on port 80" << std::endl;
        return 1;
    }
    else
        std::cout << "Server is listening on port 80" << std::endl;

    return a.exec();
}