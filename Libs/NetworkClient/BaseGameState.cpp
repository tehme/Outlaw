#include "BaseGameState.hpp"
#include "AbstractMessageHandler.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

BaseGameState::BaseGameState(QObject * parent) :
    AbstractGameState(parent)
{
}

BaseGameState::~BaseGameState()
{
    qDeleteAll(m_messageHandlers);
    m_messageHandlers.clear();
}

void BaseGameState::addMessageHandler(AbstractMessageHandler * handler)
{
    if(!handler)
    {
        return;
    }

    m_messageHandlers.append(handler);
    connect(handler, SIGNAL(outboundMessage(QByteArray)), this, SIGNAL(outboundMessage(QByteArray)));
}

void BaseGameState::removeMessageHandler(AbstractMessageHandler * handler)
{
    if(!handler || !m_messageHandlers.contains(handler))
    {
        return;
    }

    m_messageHandlers.removeOne(handler);
    delete handler;
}

void BaseGameState::onInboundMessage(QByteArray data)
{
    for(AbstractMessageHandler * handler : m_messageHandlers)
    {
        handler->onInboundMessage(static_cast<int>(getServerState()), data);
    }
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
