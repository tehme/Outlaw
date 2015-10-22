#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "MessageBuffer.hpp"
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED TcpClient : public QObject
{
    Q_OBJECT

public:
    TcpClient(QObject * parent = nullptr);
    virtual ~TcpClient();

    void connectToHost(const QString & host, const quint16 port);
    void disconnectFromHost();

signals:
    void messageRead(QByteArray data);
    void connected();
    void disconnected();
    void socketError(QAbstractSocket::SocketError socketError);

public slots:
    void writeMessage(QByteArray data);
    void setCompressionThreshold(int threshold);

private slots:
    void onSocketReadyRead();

private:
    void handleMessages();
    bool handleNextMessage();

private:
    QTcpSocket    m_socket;
    QString       m_host;
    ushort        m_port;
    int           m_compressionThreshold;

    MessageBuffer m_incomingMessagesBuffer;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // TCPCLIENT_HPP

//----------------------------------------------------------------------------//
