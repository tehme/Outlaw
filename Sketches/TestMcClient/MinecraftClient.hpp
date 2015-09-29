#ifndef MINECRAFTCLIENT_H
#define MINECRAFTCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

class MinecraftClient : public QObject
{
    Q_OBJECT

public:
    MinecraftClient(QObject *parent = nullptr);
    virtual ~MinecraftClient();

    void connectToHost(const QString &host, const quint16 port);
    void sendHandshake();
    void sendServerListPing();

private slots:
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSocketReadyRead();

private:
    // This should be a slot.
    void handleMessages();
    bool handleNextMessage();

private:
    QTcpSocket    m_socket;
    QString       m_host;
    ushort        m_port;

    MessageBuffer m_incomingMessagesBuffer;
};

//----------------------------------------------------------------------------//

#endif // MINECRAFTCLIENT_H
