#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// This part will be moved to BaseGameState
// {
#include <QPointer>
#include <QVector>
#include <QString>
// }
#include <NetworkClient/AbstractGameState.hpp>

//----------------------------------------------------------------------------//

namespace NetworkClient
{
    class TcpClient;
}

//----------------------------------------------------------------------------//

// This part will be moved to NetworkClient
// {
class AbstractMessageHandler : public QObject
{
    Q_OBJECT

public:
    AbstractMessageHandler(QObject * parent = nullptr);
    virtual ~AbstractMessageHandler() override;

signals:
    void outboundMessage(QByteArray data);

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) = 0;
};


// This handler is only responsive for sending keep-alive messages in Play state.
// It does not log in by itself.
class KeepAliveHandler : public AbstractMessageHandler
{
    Q_OBJECT

public:
    KeepAliveHandler(QObject * parent = nullptr);
    virtual ~KeepAliveHandler() override;

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};


class LoginHandler : public AbstractMessageHandler
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

class GameState : public NetworkClient::AbstractGameState
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
    QPointer<AbstractMessageHandler> addMessageHandler(AbstractMessageHandler * handler);
    void removeMessageHandler(AbstractMessageHandler * handler);

signals:
    void serverStateChanged(int newState);

protected:
    NetworkClient::ServerState getServerState() const;
    void setServerState(NetworkClient::ServerState newState);

private:
    NetworkClient::ServerState        m_serverState = NetworkClient::ServerState::Undefined;
    QVector<AbstractMessageHandler *> m_messageHandlers;
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
