#include "GameState.hpp"
#include <NetworkClient/MessageBuffer.hpp>
#include <NetworkClient/TcpClient.hpp>
#include <NetworkClient/VarInt.hpp>

//----------------------------------------------------------------------------//

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

KeepAliveHandler::KeepAliveHandler(QObject * parent) :
    AbstractMessageHandler(parent)
{
}

KeepAliveHandler::~KeepAliveHandler()
{
}

void KeepAliveHandler::onInboundMessage(int serverState, QByteArray data)
{
    nc::ServerState trueState = static_cast<nc::ServerState>(serverState);

    if(trueState != nc::ServerState::Play)
    {
        return;
    }

    nc::MessageBuffer buffer(data);
    nc::VarInt messageCode;

    buffer >> messageCode;

    if(messageCode.getValue() != 0x00) // keep-alive
    {
        return;
    }

    // Send data back, as clientbound and serverbound packets are equal.
    emit outboundMessage(data);
}

//----------------------------------------------------------------------------//

LoginHandler::LoginHandler(QObject * parent) :
    AbstractMessageHandler(parent)
{
}

void LoginHandler::onInboundMessage(int serverState, QByteArray data)
{
    nc::ServerState trueState = static_cast<nc::ServerState>(serverState);

    if(trueState != nc::ServerState::Login)
    {
        return;
    }

    nc::MessageBuffer buffer(data);
    nc::VarInt messageCode;

    buffer >> messageCode;

    if(messageCode.getValue() == 0x02) // login success
    {
        emit loginFinished();
    }
    else if(messageCode.getValue() == 0x03) // set compression
    {
        nc::VarInt threshold;
        buffer >> threshold;

        emit compressionThresholdChanged(threshold.getValue());
    }
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

GameState::GameState(
    const QString            & host,
    quint16                    port,
    const QString            & userName,
    NetworkClient::TcpClient & tcpClient) :

    NetworkClient::BaseGameState(nullptr),
    m_host(host),
    m_port(port),
    m_userName(userName)
{
    LoginHandler * loginHandler = new LoginHandler;
    // Special signals are connected manually for now.
    connect(loginHandler, SIGNAL(compressionThresholdChanged(int)), &tcpClient, SLOT(setCompressionThreshold(int)));
    connect(loginHandler, SIGNAL(loginFinished()), this, SLOT(onLoginFinished()));

    addMessageHandler(loginHandler);
    addMessageHandler(new KeepAliveHandler);
}

GameState::~GameState()
{
    qDeleteAll(m_messageHandlers);
    m_messageHandlers.clear();
}

void GameState::run()
{
    if(getServerState() != nc::ServerState::Undefined)
    {
        qWarning() << "GameState: called sendLoginHandshake() in state" << static_cast<int>(getServerState());
        return;
    }

    setServerState(nc::ServerState::Login);

    sendLoginHandshake();
    sendLoginStart();
}

void GameState::onInboundMessage(QByteArray data)
{
    for(nc::AbstractMessageHandler * handler : m_messageHandlers)
    {
        handler->onInboundMessage(static_cast<int>(getServerState()), data);
    }
}

void GameState::addMessageHandler(nc::AbstractMessageHandler * handler)
{
    if(!handler)
    {
        return;
    }

    m_messageHandlers.append(handler);
    connect(handler, SIGNAL(outboundMessage(QByteArray)), this, SIGNAL(outboundMessage(QByteArray)));
}

void GameState::removeMessageHandler(nc::AbstractMessageHandler * handler)
{
    if(!handler || !m_messageHandlers.contains(handler))
    {
        return;
    }

    m_messageHandlers.removeOne(handler);
    delete handler;
}


//----------------------------------------------------------------------------//

void GameState::onLoginFinished()
{
    if(getServerState() == nc::ServerState::Login)
    {
        setServerState(nc::ServerState::Play);
    }
    else
    {
        qWarning() << "GameState: called onLoginFinished() slot in state" << static_cast<int>(getServerState());
    }
}

void GameState::sendLoginHandshake()
{
    if(getServerState() != nc::ServerState::Login)
    {
        qWarning() << "GameState: called sendLoginHandshake() in state" << static_cast<int>(getServerState());
        return;
    }

    nc::MessageBuffer buffer;
    buffer
        << nc::VarInt(0x00) // packet id
        << nc::VarInt(47)   // protocol version (1.8.8)
        << QString(m_host)  // server address
        << ushort(m_port)   // server port
        << nc::VarInt(2);   // next state: login

    QByteArray bufferBytes = buffer.getAllBytes();
    emit outboundMessage(bufferBytes);
}

void GameState::sendLoginStart()
{
    if(getServerState() != nc::ServerState::Login)
    {
        qWarning() << "GameState: called sendLoginStart() in state" << static_cast<int>(getServerState());
        return;
    }

    nc::MessageBuffer buffer;

    buffer
        << nc::VarInt(0x00)
        << QString(m_userName);

    QByteArray bufferBytes = buffer.getAllBytes();
    emit outboundMessage(bufferBytes);
}

//----------------------------------------------------------------------------//
