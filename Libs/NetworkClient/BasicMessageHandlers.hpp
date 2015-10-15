#ifndef BASICMESSAGEHANDLERS_HPP
#define BASICMESSAGEHANDLERS_HPP

#include <QObject>
#include "AbstractMessageHandler.hpp"
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED KeepAliveHandler : public AbstractMessageHandler
{
    Q_OBJECT

public:
    KeepAliveHandler(QObject * parent = nullptr);
    virtual ~KeepAliveHandler() override;

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED LoginHandler : public AbstractMessageHandler
{
    Q_OBJECT

public:
    LoginHandler(QObject * parent = nullptr);

signals:
    void compressionThresholdChanged(int threshold);
    void loginFinished();

public slots:
    virtual void onInboundMessage(int serverState, QByteArray data) override;
};

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // BASICMESSAGEHANDLERS_HPP

//----------------------------------------------------------------------------//
