#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

class TcpClient : public QObject
{
    Q_OBJECT

public:
    TcpClient(QObject * parent = nullptr);
    virtual ~TcpClient();

    void connectToHost(const QString & host, const quint16 port);
    void sendServerListPing();

signals:
    void messageRead(QByteArray data);

public slots:
    void writeMessage(QByteArray data);

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

#endif // TCPCLIENT_H
