#ifndef ABSTRACTGAMESTATE_HPP
#define ABSTRACTGAMESTATE_HPP

#include <QObject>
#include <QByteArray>
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

enum class ServerState
{
    Status,
    Login,
    Play,

    Undefined
};

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED AbstractGameState : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGameState(QObject * parent = nullptr);
    virtual ~AbstractGameState() override;

signals:
    // Data should be without length bytes.
    void outboundMessage(QByteArray data);

public slots:
    virtual void onInboundMessage(QByteArray data) = 0;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // ABSTRACTGAMESTATE_HPP

//----------------------------------------------------------------------------//
