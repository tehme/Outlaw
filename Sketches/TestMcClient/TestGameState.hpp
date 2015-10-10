#ifndef TESTGAMESTATE_HPP
#define TESTGAMESTATE_HPP

#include <NetworkClient/BaseGameState.hpp>
#include <NetworkClient/MessageBuffer.hpp>

//----------------------------------------------------------------------------//

class TestGameState : public NetworkClient::BaseGameState
{
    Q_OBJECT

public:
    explicit TestGameState(QObject * parent = nullptr);
    virtual ~TestGameState() override;

    void setHost(const QString & host);
    void setPort(quint16 port);
    void setUserName(const QString & userName);

signals:
    void entitySpawned(int entityId, int x, int y, int z);
    void entityDestroyed(int entityId);
    void entityPositionChanged(int entityId, int x, int y, int z, bool isRelative);


public slots:
    virtual void onInboundMessage(QByteArray data) override;

    void run();

private:
    void sendLoginHandshake();
    void sendLoginStart();

    void tryHandleEntityMessage(int messageCode, NetworkClient::MessageBuffer & buffer);

private:
    NetworkClient::ServerState m_serverState; // Move to base class?

    // TODO: request from TcpClient or store here separately? Or even make parent object that knows this?
    QString    m_host;
    quint16    m_port;
    QString    m_userName;
};

//----------------------------------------------------------------------------//

#endif // TESTGAMESTATE_HPP
