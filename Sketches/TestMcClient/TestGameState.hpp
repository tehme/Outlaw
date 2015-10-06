#ifndef TESTGAMESTATE_HPP
#define TESTGAMESTATE_HPP

#include "BaseGameState.hpp"

//----------------------------------------------------------------------------//

class TestGameState : public BaseGameState
{
    Q_OBJECT

public:
    explicit TestGameState(QObject * parent = nullptr);
    virtual ~TestGameState() override;

    void setHost(const QString & host);
    void setPort(quint16 port);
    void setUserName(const QString & userName);

public slots:
    virtual void onMessageReceived(QByteArray data) override;

    void run();

private:
    void sendLoginHandshake();
    void sendLoginStart();

private:
    ServerState m_serverState; // Move to base class?

    // TODO: request from TcpClient or store here separately? Or even make parent object that knows this?
    QString    m_host;
    quint16    m_port;
    QString    m_userName;
};

//----------------------------------------------------------------------------//

#endif // TESTGAMESTATE_HPP
