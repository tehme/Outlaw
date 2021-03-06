#ifndef BASEGAMESTATE_HPP
#define BASEGAMESTATE_HPP

#include <QVector>
#include "AbstractGameState.hpp"
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class AbstractMessageHandler;

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED BaseGameState : public AbstractGameState
{
    Q_OBJECT

public:
    explicit BaseGameState(QObject * parent = nullptr);
    virtual ~BaseGameState() override;

    void addMessageHandler(AbstractMessageHandler * handler);
    void removeMessageHandler(AbstractMessageHandler * handler);

signals:
    void serverStateChanged(int newState);

public slots:
    virtual void onInboundMessage(QByteArray data) override;

protected:
    ServerState getServerState() const;
    void setServerState(ServerState newState);

    virtual void preMessageHandle();
    virtual void postMessageHandle();

private:
    ServerState                       m_serverState = ServerState::Undefined;
    QVector<AbstractMessageHandler *> m_messageHandlers;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // BASEGAMESTATE_HPP

//----------------------------------------------------------------------------//
