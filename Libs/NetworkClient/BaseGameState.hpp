#ifndef BASEGAMESTATE_HPP
#define BASEGAMESTATE_HPP

#include "AbstractGameState.hpp"
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED BaseGameState : public AbstractGameState
{
    Q_OBJECT

public:
    explicit BaseGameState(QObject * parent = nullptr);
    virtual ~BaseGameState() override;

signals:
    void serverStateChanged(int newState);

protected:
    ServerState getServerState() const;
    void setServerState(ServerState newState);

private:
    ServerState m_serverState = ServerState::Undefined;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // BASEGAMESTATE_HPP

//----------------------------------------------------------------------------//
