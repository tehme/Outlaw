#include <QApplication>
#include <QTcpSocket>
#include <QDebug>
#include "TcpClient.hpp"
#include "TestGameState.hpp"

#include "MessageBuffer.hpp"
#include "VarInt.hpp"
#include "BinaryUtils.hpp"
#include "MobRadarWidget.hpp"

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    qDebug() << "It works!";

    const QString host = "localhost";
    const ushort  port = 25565;

    TcpClient client;
    TestGameState gameState;

    QObject::connect(&client, SIGNAL(messageRead(QByteArray)), &gameState, SLOT(onMessageReceived(QByteArray)));
    QObject::connect(&gameState, SIGNAL(messageSent(QByteArray)), &client, SLOT(writeMessage(QByteArray)));

    MobRadarWidget mobRadarWidget(0, 0, 1);
    QObject::connect(
        &gameState,      SIGNAL(entitySpawned(int,int,int,int)),
        &mobRadarWidget, SLOT(onEntitySpawned(int,int,int,int)));
    QObject::connect(
        &gameState,      SIGNAL(entityDestroyed(int)),
        &mobRadarWidget, SLOT(onEntityDestroyed(int)));
    QObject::connect(
        &gameState,      SIGNAL(entityPositionChanged(int,int,int,int,bool)),
        &mobRadarWidget, SLOT(onEntityPositionChanged(int,int,int,int,bool)));


    mobRadarWidget.show();

    client.connectToHost(host, port);

    gameState.setHost(host);
    gameState.setPort(port);
    gameState.setUserName("tehme");
    gameState.run();

    freopen("output.txt","w", stdout);

    return a.exec();
}



