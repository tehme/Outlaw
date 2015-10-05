#ifndef BASEGAMESTATE_HPP
#define BASEGAMESTATE_HPP

#include <QObject>
#include <QByteArray>

//----------------------------------------------------------------------------//

enum class ServerState
{
    Status,
    Login,
    Play,

    Undefined
};

//----------------------------------------------------------------------------//

class BaseGameState : public QObject
{
    Q_OBJECT

public:
    explicit BaseGameState(QObject * parent = nullptr);
    virtual ~BaseGameState() override;

signals:
    void messageSent(QByteArray data);

public slots:
    virtual void onMessageReceived(QByteArray data) = 0;
};

//----------------------------------------------------------------------------//

#endif // BASEGAMESTATE_HPP
