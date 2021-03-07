#include "http_server.h"
#include <QtCore/QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Connection::init();
    

    http_server *serv = new http_server(&a); // 10.0.0.4

    if (!serv->isListening()) {
        std::cout << "Server is not listening" << std::endl;
        return 1;
    }
    else
        std::cout << "Server is listening" << std::endl;

    return a.exec();
}