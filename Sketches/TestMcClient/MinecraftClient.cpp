#include "MinecraftClient.hpp"
#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------//

MinecraftClient::MinecraftClient(QObject *parent) :
    QObject(parent),
    m_socket()
{
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

MinecraftClient::~MinecraftClient()
{
}

void MinecraftClient::connectToHost(const QString &host, const quint16 port)
{
    m_socket.connectToHost(host, port);
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

    QByteArray handshakeData;

    // packet length
    handshakeData.append(char(14));

    // packet id
    handshakeData.append(char(0x00));

    // protocol version
    handshakeData.append(char(47));

    // server address
    handshakeData.append(char(8));           // length
    handshakeData.append("localhost"); // string

    // server port
    handshakeData.append(char(25565 >> 8));
    handshakeData.append(char(25565 & 256));

    // next state
    handshakeData.append(char(1));

    for(auto c : handshakeData)
    {
        std::cout << std::hex << int(c) << " ";
    }

    std::cout << std::endl;
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

    for(auto c : data)
    {
        std::cout << std::hex << int(c) << " ";
    }

    std::cout << std::endl;
}

//----------------------------------------------------------------------------//
