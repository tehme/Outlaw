#include "MinecraftClient.hpp"
#include <iostream>
#include <iomanip>
#include "BinaryUtils.hpp"

//----------------------------------------------------------------------------//

MinecraftClient::MinecraftClient(QObject *parent) :
    QObject(parent),
    m_socket(),
    m_host(),
    m_port(0),
    m_serverState(ServerState::Undefined)
{
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
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

    MessageBuffer handshakeSizeBuffer;
    MessageBuffer handshakeBuffer;

    handshakeBuffer
        << VarInt(0x00)    // packet id
        << VarInt(47)      // protocol version
        << QString(m_host) // server address
        << ushort(m_port)  // server port
        << VarInt(2);      // next state: login

    handshakeSizeBuffer << VarInt(handshakeBuffer.getSize());

    std::cout << "Handshake data:" << std::endl;
    BinaryUtils::dumpHex(handshakeBuffer.getAllBytes());
    std::cout << "Handshake data end" << std::endl;

    m_socket.write(handshakeSizeBuffer.getAllBytes());
    m_socket.write(handshakeBuffer.getAllBytes());

    /*
    Packet ID 	State 	Bound To 	Field Name 	Field Type 	Notes
    0x00 	Login 	Server 	Name 	String
    */

    MessageBuffer loginStartSizeBuffer;
    MessageBuffer loginStartBuffer;

    loginStartBuffer
        << VarInt(0x00)
        << QString("tehme");

    loginStartSizeBuffer << VarInt(loginStartBuffer.getSize());

    m_socket.write(loginStartSizeBuffer.getAllBytes());
    m_socket.write(loginStartBuffer.getAllBytes());

    m_socket.flush();

    m_serverState = ServerState::Login;
}

void MinecraftClient::sendServerListPing()
{
    QByteArray data;
    data.append(char(0xfe));
    data.append(char(0x01));
    m_socket.write(data);
}

//----------------------------------------------------------------------------//

void MinecraftClient::onSocketConnected()
{
    qDebug() << "Socket connected!";
}

void MinecraftClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error!" << socketError;
}

void MinecraftClient::onSocketReadyRead()
{
    QByteArray data = m_socket.readAll();
    m_incomingMessagesBuffer.writeBytesToBuffer(data);
    handleMessages(); // emit signal to handler/state object.
}

//----------------------------------------------------------------------------//

void MinecraftClient::handleMessages()
{
//    std::cout << "Response data:" << std::endl;

//    BinaryUtils::dumpHex(data);

//    MessageBuffer buffer(data);
//    VarInt msgSize;
//    buffer >> msgSize;

//    if(buffer.getSize() >= msgSize.getValue())
//    {
//        VarInt msgCode;
//        buffer >> msgCode;

//        if(msgCode.getValue() == 0x00) // status
//        {
//            QString statusText;
//            buffer >> statusText;
//            qDebug() << statusText;
//        }
//    }

//    std::cout << std::endl << "Response data end" << std::endl;

//    std::cout << "Received data dump:" << std::endl;
//    BinaryUtils::dumpHex(m_incomingMessagesBuffer.getAllBytes());
//    std::cout << "Received data dump end" << std::endl;

    while(m_incomingMessagesBuffer.getSize() > 0 && handleNextMessage())
    {
        std::cout << "Handled message!" << std::endl;
    }

    m_incomingMessagesBuffer.clearToOffset();
}

bool MinecraftClient::handleNextMessage()
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
        std::cout << "Not enough data!" << std::endl;
        // Not enough data, return offset and wait for more data.
        // TODO: bytes rollback function.
        m_incomingMessagesBuffer.moveOffset(-messageSize.getSizeAsArray());
        return false;
    }

    VarInt messageCode;
    m_incomingMessagesBuffer >> messageCode;

    std::cout
        << "Message code: " << std::hex << messageCode.getValue() << std::endl
        << "Offset: " << std::dec << m_incomingMessagesBuffer.getOffset() << " of " << m_incomingMessagesBuffer.getSize()
        << std::endl;

    if(m_serverState == ServerState::Login && messageCode.getValue() == 0x02)
    {
        m_serverState = ServerState::Play;

        const int nBytesToSkip = messageSize.getValue() - messageCode.getSizeAsArray();
        std::cout << "Skipping " << messageSize.getValue() << " bytes (login success)." << std::endl;
        m_incomingMessagesBuffer.moveOffset(nBytesToSkip);
    }
    else if(m_serverState == ServerState::Play && messageCode.getValue() == 0x00) // keep-alive
    {
        // TODO: schedule sending? Or does socket already do it?
        VarInt keepAliveId;
        m_incomingMessagesBuffer >> keepAliveId;

        std::cout << "Got keep-alive with id " << keepAliveId.getValue() << ". Sending it back." << std::endl;

        MessageBuffer buffer;
        buffer
            << VarInt(1 + keepAliveId.getSizeAsArray()) // size: code + keep-alive id. TODO: function to write to socket with size.
            << VarInt(0x00)
            << keepAliveId;

        m_socket.write(buffer.getAllBytes());
    }
    else if(/*messageCode.getValue() == -1*/ false)
    {

    }
    else
    {
        // Skip
        const int nBytesToSkip = messageSize.getValue() - messageCode.getSizeAsArray();
        std::cout << "Skipping " << messageSize.getValue() << " bytes." << std::endl;
        m_incomingMessagesBuffer.moveOffset(nBytesToSkip);
    }

    return true;

}

bool MinecraftClient::isMessageSupported(ServerState currentServerState, int code)
{
    // TODO: query handlers instead of hard-coding this.
    if(currentServerState == ServerState::Login && code == 0x02) // this sets server state to Play
        return true;

    return false;
}

//----------------------------------------------------------------------------//
