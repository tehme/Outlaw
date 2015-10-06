#include "TestGameState.hpp"
#include <iostream>
#include <QByteArray>
#include <QString>
#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

TestGameState::TestGameState(QObject * parent) :
    BaseGameState(parent),
    m_serverState(ServerState::Undefined),
    m_host(),
    m_port(0),
    m_userName()
{
}

TestGameState::~TestGameState()
{
}

void TestGameState::setHost(const QString & host)
{
    m_host = host;
}

void TestGameState::setPort(quint16 port)
{
    m_port = port;
}

void TestGameState::setUserName(const QString & userName)
{
    m_userName = userName;
}

//----------------------------------------------------------------------------//

void TestGameState::onMessageReceived(QByteArray data)
{
    MessageBuffer buffer(data);

    VarInt messageCode;
    buffer >> messageCode;

    std::cout
        << "Message received! Size: " << data.size()
        << "; code: " << messageCode.getValue()
        << "; game state: " << static_cast<int>(m_serverState)
        << std::endl;

    if(m_serverState == ServerState::Login)
    {
        if(messageCode.getValue() == 0x02) // login success
        {
            m_serverState = ServerState::Play;
        }
    }
    else if(m_serverState == ServerState::Play)
    {
        if(messageCode.getValue() == 0x00) // keep-alive
        {
            // Echo keep-alive back, as serverbound message is equal to clientbound one (prot. v47).
            std::cout << "Got keep-alive!" << std::endl;
            emit messageSent(data);
        }
    }
}

void TestGameState::run()
{
    sendLoginHandshake();
    sendLoginStart();
}

//----------------------------------------------------------------------------//

void TestGameState::sendLoginHandshake()
{
    if(m_serverState != ServerState::Undefined)
    {
        // TODO: warning here
        return;
    }

    m_serverState = ServerState::Login;

    MessageBuffer buffer;

    buffer
        << VarInt(0x00)    // packet id
        << VarInt(47)      // protocol version
        << QString(m_host) // server address
        << ushort(m_port)  // server port
        << VarInt(2);      // next state: login

    QByteArray bufferBytes = buffer.getAllBytes();
    emit messageSent(bufferBytes);
}

void TestGameState::sendLoginStart()
{
    MessageBuffer buffer;

    buffer
        << VarInt(0x00)
        << QString(m_userName);

    QByteArray bufferBytes = buffer.getAllBytes();
    emit messageSent(bufferBytes);
}

//----------------------------------------------------------------------------//
