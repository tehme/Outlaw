#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "MinecraftClient.hpp"

#include "MessageBuffer.hpp"
#include "VarInt.hpp"

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    qDebug() << "It works!";

//    MinecraftClient client;
//    client.connectToHost("localhost", 25565);
//    client.sendHandshake();

    MessageBuffer buffer;

    QString str="herro!";
    buffer << str;
    str.clear();
    buffer >> str;

    qDebug() << str;

    {
        QDebug deb = qDebug();
        for(unsigned char c : buffer.getAllBytes())
        {
            deb << QString::number(c, 16);
        }
    }

    return a.exec();
}



