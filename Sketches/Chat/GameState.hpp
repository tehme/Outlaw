#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// This part will be moved to BaseGameState
// {
#include <QPointer>
#include <QVector>
#include <QString>
// }
#include <NetworkClient/BaseGameState.hpp>
#include <NetworkClient/AbstractMessageHandler.hpp>

//----------------------------------------------------------------------------//

namespace NetworkClient
{
    class TcpClient;
}

namespace nc = NetworkClient;

//----------------------------------------------------------------------------//

// This part will be moved to NetworkClient
// {

// This handler is only responsive for sending keep-alive messages in Play state.
// It does not log in by itself.
class KeepAliveHandler : public nc::AbstractMessageHandler
{
    Q_OBJECT

public:
    KeepAliveHandler(QObject * parent = nullptr);
    virtual ~KeepAliveHandler() override;

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};


class LoginHandler : public nc::AbstractMessageHandler
{
    Q_OBJECT

public:
    LoginHandler(QObject * parent = nullptr);

signals:
    void compressionThresholdChanged(int threshold);
    void loginFinished();

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};
// }

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

signals:

public slots:
    virtual void onInboundMessage(QByteArray data) override;

// This part will be moved to BaseGameState
// {
public:
    void addMessageHandler(nc::AbstractMessageHandler * handler);
    void removeMessageHandler(nc::AbstractMessageHandler * handler);

private:
    QVector<nc::AbstractMessageHandler *> m_messageHandlers;
// }

private slots:
    void onLoginFinished();

private:
    void sendLoginHandshake();
    void sendLoginStart();

private:
    QString m_host;
    quint16 m_port;
    QString m_userName;
};

//----------------------------------------------------------------------------//

#endif // GAMESTATE_HPP
