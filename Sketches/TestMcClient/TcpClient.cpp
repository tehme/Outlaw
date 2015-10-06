#include "TcpClient.hpp"
#include <iostream>
#include <iomanip>
#include "BinaryUtils.hpp"

//----------------------------------------------------------------------------//

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    m_socket(),
    m_host(),
    m_port(0)
{
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

TcpClient::~TcpClient()
{
}

void TcpClient::connectToHost(const QString & host, const quint16 port)
{
    m_host = host;
    m_port = port;
    m_socket.connectToHost(host, port);

    m_socket.waitForConnected(); // TODO: remove this and re-emit signals.
}

void TcpClient::sendServerListPing()
{
    QByteArray data;
    data.append(char(0xfe));
    data.append(char(0x01));
    m_socket.write(data);
}

//----------------------------------------------------------------------------//

void TcpClient::writeMessage(QByteArray data)
{
    MessageBuffer sizeBuffer;
    sizeBuffer << VarInt(data.size());

    m_socket.write(sizeBuffer.getAllBytes());
    m_socket.write(data);
    m_socket.flush();
}

//----------------------------------------------------------------------------//

void TcpClient::onSocketConnected()
{
    qDebug() << "Socket connected!";
}

void TcpClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error!" << socketError;
}

void TcpClient::onSocketReadyRead()
{
    QByteArray data = m_socket.readAll();
    m_incomingMessagesBuffer.writeBytesToBuffer(data);
    handleMessages();
}

//----------------------------------------------------------------------------//

void TcpClient::handleMessages()
{
    while(m_incomingMessagesBuffer.getSize() > 0 && handleNextMessage())
    {
//        std::cout << "Handled message!" << std::endl;
    }

    m_incomingMessagesBuffer.clearToOffset();
}

bool TcpClient::handleNextMessage()
{
    VarInt messageSize;

    // TODO: check for possibility to read VarInt without try/catch.
    try
    {
        m_incomingMessagesBuffer >> messageSize;
    }
    catch(...)
    {
        std::cout << "Caught exception!" << std::endl;
        return false;
    }

    if(m_incomingMessagesBuffer.getSize() < messageSize.getValue())
    {
//        std::cout << "Not enough data!" << std::endl;
        // Not enough data, return offset and wait for more data.
        // TODO: bytes rollback function.
        m_incomingMessagesBuffer.moveOffset(-messageSize.getSizeAsArray());
        return false;
    }

    QByteArray messageBytes = m_incomingMessagesBuffer.readBytesFromBuffer(messageSize.getValue());

    emit messageRead(messageBytes);

    return true;
}

//----------------------------------------------------------------------------//
