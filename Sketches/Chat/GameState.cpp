#include "GameState.hpp"
#include <QDebug>
#include <NetworkClient/MessageBuffer.hpp>
#include <NetworkClient/TcpClient.hpp>
#include <NetworkClient/VarInt.hpp>
#include <NetworkClient/BasicMessageHandlers.hpp>

//----------------------------------------------------------------------------//

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

ChatHandler::ChatHandler(QObject * parent) :
    nc::AbstractMessageHandler(parent)
{
}

ChatHandler::~ChatHandler()
{
}

void ChatHandler::onInboundMessage(int serverState, QByteArray data)
{
    nc::ServerState trueState = static_cast<nc::ServerState>(serverState);

    if(trueState != nc::ServerState::Play)
    {
        return;
    }

    nc::MessageBuffer buffer(data);
    nc::VarInt messageCode;

    buffer >> messageCode;

    if(messageCode.getValue() != 0x02) // chat message
    {
        return;
    }

    QString chatMessage;
    buffer >> chatMessage;

    qDebug() << "Chat message!";
    qDebug() << chatMessage;

    emit chatMessageReceived(chatMessage);
}


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
    nc::LoginHandler * loginHandler = new nc::LoginHandler;
    // Special signals are connected manually for now.
    connect(loginHandler, SIGNAL(compressionThresholdChanged(int)), &tcpClient, SLOT(setCompressionThreshold(int)));
    connect(loginHandler, SIGNAL(loginFinished()), this, SLOT(onLoginFinished()));
    addMessageHandler(loginHandler);

    addMessageHandler(new nc::KeepAliveHandler);

    ChatHandler * chatHandler = new ChatHandler;
    connect(chatHandler, SIGNAL(chatMessageReceived(QString)), this, SIGNAL(chatMessageReceived(QString)));
    addMessageHandler(chatHandler);
}

GameState::~GameState()
{
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

void GameState::onChatMessageSent(QString chatMessage)
{
    if(getServerState() != nc::ServerState::Play)
    {
        return;
    }

    nc::MessageBuffer buffer;
    buffer << nc::VarInt(0x01) << chatMessage;

    emit outboundMessage(buffer.getAllBytes());
}

//----------------------------------------------------------------------------//

void GameState::onLoginFinished()
{
    if(getServerState() == nc::ServerState::Login)
    {
        m_playStateScheduled = true;
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

void GameState::postMessageHandle()
{
    if(m_playStateScheduled)
    {
        m_playStateScheduled = false;
        setServerState(nc::ServerState::Play);
    }
}

//----------------------------------------------------------------------------//
