#ifndef ABSTRACTMESSAGEHANDLER_HPP
#define ABSTRACTMESSAGEHANDLER_HPP

#include <QObject>
#include <QByteArray>
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED AbstractMessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit AbstractMessageHandler(QObject *parent = nullptr);
    virtual ~AbstractMessageHandler() override;

signals:
    void outboundMessage(QByteArray data);

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) = 0;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // ABSTRACTMESSAGEHANDLER_HPP

//----------------------------------------------------------------------------//
