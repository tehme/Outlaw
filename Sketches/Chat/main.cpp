#include "MainWindow.hpp"
#include <QApplication>
#include <NetworkClient/TcpClient.hpp>
#include "GameState.hpp"

//----------------------------------------------------------------------------//

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString host = "127.0.0.1";
    quint16 port = 25565;
    QString name = "tehme";

    nc::TcpClient tcpClient;
    GameState gameState(host, port, name, tcpClient);

    QObject::connect(&tcpClient, SIGNAL(messageRead(QByteArray)), &gameState, SLOT(onInboundMessage(QByteArray)));
    QObject::connect(&gameState, SIGNAL(outboundMessage(QByteArray)), &tcpClient, SLOT(writeMessage(QByteArray)));
    QObject::connect(&gameState, SIGNAL(chatMessageReceived(QString)), &w, SLOT(onChatMessageReceived(QString)));
    QObject::connect(&w, SIGNAL(chatMessageSent(QString)), &gameState, SLOT(onChatMessageSent(QString)));

    tcpClient.connectToHost(host, port);
    gameState.run();

    return a.exec();
}

//----------------------------------------------------------------------------//
