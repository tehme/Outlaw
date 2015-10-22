#include "TcpClient.hpp"
#include <iostream>
#include <iomanip>
#include <QtEndian>
#include "VarInt.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    m_socket(),
    m_host(),
    m_port(0),
    m_compressionThreshold(-1)
{
    connect(&m_socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(socketError(QAbstractSocket::SocketError)));

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
}

void TcpClient::disconnectFromHost()
{
    m_host = QString();
    m_port = 0;
    m_socket.disconnectFromHost();
}

//----------------------------------------------------------------------------//

void TcpClient::writeMessage(QByteArray data)
{
    MessageBuffer sizeBuffer;

    const bool isCompressionEnabled = (m_compressionThreshold > -1);

    if(isCompressionEnabled)
    {
        VarInt uncompressedSize;

        if(data.size() >= m_compressionThreshold)
        {
            uncompressedSize.setValue(data.size());
            data = qCompress(data);
        }
        else
        {
            uncompressedSize.setValue(0);
        }

        // Full packet size; uncompressed data size
        sizeBuffer << VarInt(data.size() + uncompressedSize.getSizeAsArray()) << uncompressedSize;
    }
    else
    {
        sizeBuffer << VarInt(data.size());
    }

    m_socket.write(sizeBuffer.getAllBytes());
    m_socket.write(data);
    m_socket.flush();
}

void TcpClient::setCompressionThreshold(int threshold)
{
    m_compressionThreshold = threshold;
}

//----------------------------------------------------------------------------//

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

    QByteArray messageBytes;

    const bool isCompressionEnabled = (m_compressionThreshold > -1);
    if(isCompressionEnabled)
    {
        VarInt decompressedSize;
        m_incomingMessagesBuffer >> decompressedSize;

        const bool isCompressed = (decompressedSize.getValue() != 0);
        const int nBytesToRead = messageSize.getValue() - decompressedSize.getSizeAsArray();

        messageBytes = m_incomingMessagesBuffer.readBytesFromBuffer(nBytesToRead);

        if(isCompressed)
        {
            uchar compressedSize[4];
            qToBigEndian(messageBytes.size(), compressedSize);

            messageBytes.prepend(reinterpret_cast<char *>(compressedSize), 4);
            messageBytes = qUncompress(messageBytes);
        }
    }
    else
    {
        messageBytes = m_incomingMessagesBuffer.readBytesFromBuffer(messageSize.getValue());
    }

    emit messageRead(messageBytes);

    return true;
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
