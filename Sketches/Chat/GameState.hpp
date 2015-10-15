#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <NetworkClient/AbstractMessageHandler.hpp>
#include <NetworkClient/BaseGameState.hpp>

//----------------------------------------------------------------------------//

namespace NetworkClient
{
    class TcpClient;
    class AbstractMessageHandler;
}

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

class ChatHandler : public nc::AbstractMessageHandler
{
    Q_OBJECT

public:
    explicit ChatHandler(QObject * parent = nullptr);
    virtual ~ChatHandler() override;

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};

//----------------------------------------------------------------------------//

class GameState : public NetworkClient::BaseGameState
{
    Q_OBJECT

public:
    GameState(
        const QString            & host,
        quint16                    port,
        const QString            & userName,
        NetworkClient::TcpClient & tcpClient);
    virtual ~GameState() override;

    void run();

private slots:
    void onLoginFinished();

private:
    void sendLoginHandshake();
    void sendLoginStart();

    virtual void postMessageHandle() override;

private:
    QString m_host;
    quint16 m_port;
    QString m_userName;

    bool    m_playStateScheduled = false;
};

//----------------------------------------------------------------------------//

#endif // GAMESTATE_HPP
