#pragma once

#include "Connection.h"

#include <QTcpServer>

class http_server : public QTcpServer
{
    Q_OBJECT
public:
    http_server(QObject* parent);
    void incomingConnection(qintptr socket);
    void pause();
    void resume();
private slots:
    void readClient();
    void discardClient();
private:
    bool disabled;
    QString start_command(const QStringList& tokens);
    Connection conn;
};

