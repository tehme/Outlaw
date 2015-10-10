#ifndef BASEGAMESTATE_HPP
#define BASEGAMESTATE_HPP

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

class NETWORK_CLIENT_EXPORTED BaseGameState : public QObject
{
    Q_OBJECT

public:
    explicit BaseGameState(QObject * parent = nullptr);
    virtual ~BaseGameState() override;

signals:
    // Data should be without length bytes.
    void outboundMessage(QByteArray data);

public slots:
    virtual void onInboundMessage(QByteArray data) = 0;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // BASEGAMESTATE_HPP
