#include "MinecraftClient.hpp"
#include <iostream>
#include <iomanip>
#include "MessageBuffer.hpp"
#include "BinaryUtils.hpp"

//----------------------------------------------------------------------------//

MinecraftClient::MinecraftClient(QObject *parent) :
    QObject(parent),
    m_socket(),
    m_host(),
    m_port(0)
{
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

MinecraftClient::~MinecraftClient()
{
}

void MinecraftClient::connectToHost(const QString & host, const quint16 port)
{
    m_host = host;
    m_port = port;
    m_socket.connectToHost(host, port);

    m_socket.waitForConnected(); // TODO: remove this and re-emit signals.
}

void MinecraftClient::sendHandshake()
{
    /*
0 	Handshaking 	Server
Protocol Version 	VarInt 	See protocol version numbers (currently 47)
Server Address 	String 	hostname or IP, e.g. localhost or 127.0.0.1
Server Port 	Unsigned Short 	default is 25565
Next State 	VarInt 	1 for status, 2 for login
    */

    MessageBuffer sizeBuffer;
    MessageBuffer buffer;

    buffer
        << VarInt(0x00)         // packet id
        << VarInt(47)           // protocol version
        << QString(m_host)      // server address
        << ushort(m_port)       // server port
        << VarInt(1);           // next state: status

    sizeBuffer << VarInt(buffer.getSize());

    std::cout << "Handshake data:" << std::endl;
    std::cout << "Size: " << buffer.getSize() << std::endl;
    for(unsigned char c : buffer.getAllBytes())
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << int(c) << " ";
    }
    std::cout << std::endl << "Handshake data end" << std::endl;

    m_socket.write(sizeBuffer.getAllBytes());
    m_socket.write(buffer.getAllBytes());
//    m_socket.flush();

    MessageBuffer statusRequestBuffer;
    statusRequestBuffer << VarInt(1) << VarInt(0x00);

    m_socket.write(statusRequestBuffer.getAllBytes());
    m_socket.flush();
}

void MinecraftClient::sendServerListPing()
{
    QByteArray data;
    data.append(char(0xfe));
    data.append(char(0x01));
    m_socket.write(data);
}

void MinecraftClient::onSocketConnected()
{
    qDebug() << "Socket connected!";
}

void MinecraftClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error!" << socketError;
}

//----------------------------------------------------------------------------//

void MinecraftClient::readDataFromSocket()
{
    QByteArray data = m_socket.readAll();

    std::cout << "Response data:" << std::endl;

    BinaryUtils::dumpHex(data);

    MessageBuffer buffer(data);
    VarInt msgSize;
    buffer >> msgSize;

    if(buffer.getSize() >= msgSize.getNumber())
    {
        VarInt msgCode;
        buffer >> msgCode;

        if(msgCode.getNumber() == 0x00) // status
        {
            QString statusText;
            buffer >> statusText;
            qDebug() << statusText;
        }
    }


    std::cout << std::endl << "Response data end" << std::endl;
}

//----------------------------------------------------------------------------//
