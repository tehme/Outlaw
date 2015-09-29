#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "MinecraftClient.hpp"

#include "MessageBuffer.hpp"
#include "VarInt.hpp"
#include "BinaryUtils.hpp"

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    qDebug() << "It works!";

    const QString host = "localhost";
    const ushort  port = 25565;

    MinecraftClient client;
    client.connectToHost(host, port);
    client.sendHandshake();

    freopen("output.txt","w", stdout);

    return a.exec();
}



