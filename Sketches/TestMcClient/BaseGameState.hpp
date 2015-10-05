#ifndef BASEGAMESTATE_HPP
#define BASEGAMESTATE_HPP

#include <QObject>
#include <QByteArray>


class BaseGameState : public QObject
{
    Q_OBJECT

public:
    BaseGameState(QObject * parent = nullptr);
    virtual ~BaseGameState() override;

signals:
    void messageSent(QByteArray data);

public slots:
    virtual void onMessageReceived(QByteArray data) = 0;
};


#endif // BASEGAMESTATE_HPP
