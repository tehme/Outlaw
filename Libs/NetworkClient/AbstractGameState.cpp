#include "AbstractGameState.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

AbstractGameState::AbstractGameState(QObject * parent) :
    QObject(parent)
{
}

AbstractGameState::~AbstractGameState()
{
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
