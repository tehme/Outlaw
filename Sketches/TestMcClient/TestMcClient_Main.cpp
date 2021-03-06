#include <QApplication>
#include <QTcpSocket>
#include <QDebug>
#include <NetworkClient/TcpClient.hpp>
#include <NetworkClient/VarInt.hpp>
#include "TestGameState.hpp"
#include "BinaryUtils.hpp"
#include "MobRadarWidget.hpp"
#include "StatsWidget.hpp"

//----------------------------------------------------------------------------//

using NetworkClient::TcpClient;

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    qDebug() << "It works!";

    const QString host = "localhost";
    const ushort  port = 25565;

    TcpClient client;
    TestGameState gameState;

    QObject::connect(&client, SIGNAL(messageRead(QByteArray)), &gameState, SLOT(onInboundMessage(QByteArray)));
    QObject::connect(&gameState, SIGNAL(outboundMessage(QByteArray)), &client, SLOT(writeMessage(QByteArray)));
    QObject::connect(&gameState, SIGNAL(compressionThresholdChanged(int)), &client, SLOT(setCompressionThreshold(int)));

    // TODO: create separate sketch for mob radar.
    MobRadarWidget mobRadarWidget(0, 0, 1);
    QObject::connect(
        &gameState,      SIGNAL(entitySpawned(int,int,int,int,int)),
        &mobRadarWidget, SLOT(onEntitySpawned(int,int,int,int,int)));
    QObject::connect(
        &gameState,      SIGNAL(entityDestroyed(int)),
        &mobRadarWidget, SLOT(onEntityDestroyed(int)));
    QObject::connect(
        &gameState,      SIGNAL(entityPositionChanged(int,int,int,int,bool)),
        &mobRadarWidget, SLOT(onEntityPositionChanged(int,int,int,int,bool)));
    mobRadarWidget.show();

//    StatsWidget statsWidget;
//    QObject::connect(&gameState, SIGNAL(timeChanged(qint64)), &statsWidget, SLOT(onTimeChanged(qint64)));
//    QObject::connect(&gameState, SIGNAL(healthChanged(float)), &statsWidget, SLOT(onHealthChanged(float)));
//    QObject::connect(&gameState, SIGNAL(foodChanged(int,float)), &statsWidget, SLOT(onFoodChanged(int,float)));
//    statsWidget.show();

    client.connectToHost(host, port);

    gameState.setHost(host);
    gameState.setPort(port);
    gameState.setUserName("tehme");
    gameState.run();

    freopen("output.txt", "w", stdout);

    return a.exec();
}



