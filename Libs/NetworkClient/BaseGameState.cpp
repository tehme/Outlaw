#include "BaseGameState.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

BaseGameState::BaseGameState(QObject * parent) :
    AbstractGameState(parent)
{
}

BaseGameState::~BaseGameState()
{
}

ServerState BaseGameState::getServerState() const
{
    return m_serverState;
}

void BaseGameState::setServerState(ServerState newState)
{
    if(m_serverState != newState)
    {
        m_serverState = newState;
        emit serverStateChanged(static_cast<int>(newState));
    }
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
