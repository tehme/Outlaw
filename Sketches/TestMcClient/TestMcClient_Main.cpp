#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "MinecraftClient.hpp"

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "It works!";

    MinecraftClient client;
    client.connectToHost("localhost", 25565);
    client.sendServerListPing();


    return a.exec();
}



