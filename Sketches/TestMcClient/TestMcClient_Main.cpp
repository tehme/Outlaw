#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "TcpClient.hpp"
#include "TestGameState.hpp"

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

    TcpClient client;
    TestGameState gameState;

    QObject::connect(&client, SIGNAL(messageRead(QByteArray)), &gameState, SLOT(onMessageReceived(QByteArray)));
    QObject::connect(&gameState, SIGNAL(messageSent(QByteArray)), &client, SLOT(writeMessage(QByteArray)));

    client.connectToHost(host, port);

    gameState.setHost(host);
    gameState.setPort(port);
    gameState.setUserName("tehme");
    gameState.run();

    freopen("output.txt","w", stdout);

    return a.exec();
}



