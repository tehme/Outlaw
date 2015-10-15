#include "BasicMessageHandlers.hpp"
#include "AbstractGameState.hpp"
#include "MessageBuffer.hpp"
#include "VarInt.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

KeepAliveHandler::KeepAliveHandler(QObject * parent) :
    AbstractMessageHandler(parent)
{
}

KeepAliveHandler::~KeepAliveHandler()
{
}

void KeepAliveHandler::onInboundMessage(int serverState, QByteArray data)
{
    ServerState trueState = static_cast<ServerState>(serverState);

    if(trueState != ServerState::Play)
    {
        return;
    }

    MessageBuffer buffer(data);
    VarInt messageCode;

    buffer >> messageCode;

    if(messageCode.getValue() != 0x00) // keep-alive
    {
        return;
    }

    // Send data back, as clientbound and serverbound packets are equal.
    emit outboundMessage(data);
}

//----------------------------------------------------------------------------//

LoginHandler::LoginHandler(QObject * parent) :
    AbstractMessageHandler(parent)
{
}

void LoginHandler::onInboundMessage(int serverState, QByteArray data)
{
    ServerState trueState = static_cast<ServerState>(serverState);

    if(trueState != ServerState::Login)
    {
        return;
    }

    MessageBuffer buffer(data);
    VarInt messageCode;

    buffer >> messageCode;

    if(messageCode.getValue() == 0x02) // login success
    {
        emit loginFinished();
    }
    else if(messageCode.getValue() == 0x03) // set compression
    {
        VarInt threshold;
        buffer >> threshold;

        emit compressionThresholdChanged(threshold.getValue());
    }
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
