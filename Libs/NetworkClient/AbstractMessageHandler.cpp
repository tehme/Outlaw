#include "AbstractMessageHandler.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

AbstractMessageHandler::AbstractMessageHandler(QObject *parent) :
    QObject(parent)
{
}

AbstractMessageHandler::~AbstractMessageHandler()
{
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
