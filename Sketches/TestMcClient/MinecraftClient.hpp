#ifndef MINECRAFTCLIENT_H
#define MINECRAFTCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

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
    void readDataFromSocket();

private:
    QTcpSocket m_socket;
    QString    m_host;
    ushort     m_port;
};

//----------------------------------------------------------------------------//

#endif // MINECRAFTCLIENT_H
