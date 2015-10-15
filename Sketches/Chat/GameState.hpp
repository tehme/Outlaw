#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// This part will be moved to BaseGameState
// {
#include <QPointer>
#include <QVector>
#include <QString>
// }
#include <NetworkClient/BaseGameState.hpp>

//----------------------------------------------------------------------------//

namespace NetworkClient
{
    class TcpClient;
    class AbstractMessageHandler;
}

namespace nc = NetworkClient;

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
