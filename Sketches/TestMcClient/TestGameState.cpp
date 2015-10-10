#include "TestGameState.hpp"
#include <iostream>
#include <iomanip>
#include <QByteArray>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <NetworkClient/VarInt.hpp>
#include <NetworkClient/PacketsV47.hpp>

//----------------------------------------------------------------------------//

using namespace NetworkClient;
namespace cb47 = NetworkClient::PacketsV47::Clientbound;
namespace sb47 = NetworkClient::PacketsV47::Serverbound;

//----------------------------------------------------------------------------//

TestGameState::TestGameState(QObject * parent) :
    BaseGameState(parent),
    m_serverState(ServerState::Undefined),
    m_host(),
    m_port(0),
    m_userName(),
    m_tickTimer(),
    m_playerPositionInitialized(false),
    m_playerX(0.0),
    m_playerY(0.0),
    m_playerZ(0.0),
    m_playerYaw(0.0f),
    m_playerPitch(0.0f)
{
    m_tickTimer.setInterval(1000 / 20); // 20 ticks per second
    m_tickTimer.setSingleShot(false);
    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(onTickTimer()));
}

TestGameState::~TestGameState()
{
}

void TestGameState::setHost(const QString & host)
{
    m_host = host;
}

void TestGameState::setPort(quint16 port)
{
    m_port = port;
}

void TestGameState::setUserName(const QString & userName)
{
    m_userName = userName;
}

//----------------------------------------------------------------------------//

void TestGameState::onInboundMessage(QByteArray data)
{
    MessageBuffer buffer(data);

    VarInt messageCode;
    buffer >> messageCode;

    std::cout
        << "Message received! Size: " << data.size()
        << "; code: 0x" << std::hex << std::setfill('0') << std::setw(2) << messageCode.getValue()
        << std::dec << " (" << messageCode.getValue() << ")"
        << "; game state: " << static_cast<int>(m_serverState)
        << std::endl;

    if(m_serverState == ServerState::Login)
    {
        if(messageCode.getValue() == 0x02) // login success
        {
            m_serverState = ServerState::Play;
            m_tickTimer.start();
        }
        else if(messageCode.getValue() == 0x03) // set compression
        {
            VarInt threshold;
            buffer >> threshold;

            emit compressionThresholdChanged(threshold.getValue());
        }
    }
    else if(m_serverState == ServerState::Play)
    {
        if(messageCode.getValue() == 0x00) // keep-alive
        {
            // Echo keep-alive back, as serverbound message is equal to clientbound one (prot. v47).
            std::cout << "Got keep-alive!" << std::endl;
            emit outboundMessage(data);
        }
        else if(messageCode.getValue() == 0x03) // time update
        {
            qint64 worldAge, timeOfDay;
            buffer >> worldAge >> timeOfDay;

            emit timeChanged(timeOfDay);
        }
        else if(messageCode.getValue() == 0x06) // health/food update
        {
            float health;
            VarInt food;
            float saturation;

            buffer >> health >> food >> saturation;

            qDebug() << "Health changed to" << health;
            emit healthChanged(health);
            emit foodChanged(food.getValue(), saturation);
        }
        else if(messageCode.getValue() == 0x08) // player position and look
        {
            if(!m_playerPositionInitialized)
            {
                m_playerPositionInitialized = true;

                cb47::PlayerPositionAndLook posLookPacket;

                buffer >> posLookPacket;
                m_playerX = posLookPacket.m_x;
                m_playerY = posLookPacket.m_y;
                m_playerZ = posLookPacket.m_z;
                m_playerYaw = posLookPacket.m_yaw;
                m_playerPitch = posLookPacket.m_pitch;
                std::cout
                    << "Received player position and look!" << std::endl
                    << m_playerX << " " << m_playerY << " " << m_playerZ << " " << m_playerYaw << " " << m_playerPitch << std::endl;
            }
            else
            {
                // response to wrong motion. TODO: resend received values.
                std::cout << "Received player position and look as response to wrong motion" << std::endl;
            }
        }
        else if(tryHandleEntityMessage(messageCode.getValue(), buffer))
        {
        }
        else
        {
        }
    }
}

void TestGameState::run()
{
    sendLoginHandshake();
    sendLoginStart();
}

//----------------------------------------------------------------------------//

void TestGameState::sendLoginHandshake()
{
    if(m_serverState != ServerState::Undefined)
    {
        // TODO: warning here
        return;
    }

    m_serverState = ServerState::Login;

    MessageBuffer buffer;

    buffer
        << VarInt(0x00)    // packet id
        << VarInt(47)      // protocol version
        << QString(m_host) // server address
        << ushort(m_port)  // server port
        << VarInt(2);      // next state: login

    QByteArray bufferBytes = buffer.getAllBytes();
    emit outboundMessage(bufferBytes);
}

void TestGameState::sendLoginStart()
{
    MessageBuffer buffer;

    buffer
        << VarInt(0x00)
        << QString(m_userName);

    QByteArray bufferBytes = buffer.getAllBytes();
    emit outboundMessage(bufferBytes);
}

bool TestGameState::tryHandleEntityMessage(int messageCode, MessageBuffer & buffer)
{
    if(m_serverState != ServerState::Play)
    {
        //TODO: warning
        return false;
    }

    static const int spawnMobCode = 0x0F;
    static const int destroyEntitiesCode = 0x13;
    static const int entityRelativeMoveCode = 0x15;
    static const int entityLookAndRelativeMoveCode = 0x17;
    static const int entityTeleportCode = 0x18;

    static const QList<int> supportedMessages =
    {
        spawnMobCode,
        destroyEntitiesCode,
        entityRelativeMoveCode,
        entityLookAndRelativeMoveCode,
        entityTeleportCode
    };

    if(!supportedMessages.contains(messageCode))
    {
        return false;
    }

    switch(messageCode)
    {
    case spawnMobCode:
        {
            VarInt entityId;
            quint8 mobType;
            int xFixed, yFixed, zFixed;

            buffer >> entityId >> mobType >> xFixed >> yFixed >> zFixed;

            int x = double(xFixed) / 32;
            int y = double(yFixed) / 32;
            int z = double(zFixed) / 32;

            emit entitySpawned(entityId.getValue(), mobType, x, y, z);
            break;
        }

    case destroyEntitiesCode:
        {
            VarInt count, id;
            buffer >> count;

            for(int i = 0; i < count.getValue(); ++i)
            {
                buffer >> id;
                emit entityDestroyed(id.getValue());
            }

            break;
        }

    case entityRelativeMoveCode:
    case entityLookAndRelativeMoveCode:
        {
            VarInt entityId;
            qint8 deltaXFixed, deltaYFixed, deltaZFixed;

            buffer >> entityId >> deltaXFixed >> deltaYFixed >> deltaZFixed;

            int deltaX = double(deltaXFixed) / 32;
            int deltaY = double(deltaYFixed) / 32;
            int deltaZ = double(deltaZFixed) / 32;

            if(deltaX != 0 || deltaY != 0 || deltaZ != 0)
            {
                emit entityPositionChanged(entityId.getValue(), deltaX, deltaY, deltaZ, true);
            }

            break;
        }

    case entityTeleportCode:
        {
            VarInt entityId;
            int xFixed, yFixed, zFixed;

            buffer >> entityId >> xFixed >> yFixed >> zFixed;

            int x = double(xFixed) / 32;
            int y = double(yFixed) / 32;
            int z = double(zFixed) / 32;

            emit entityPositionChanged(entityId.getValue(), x, y, z, false);

            break;
        }
    }

    return true;
}

void TestGameState::onTickTimer()
{
    if(m_playerPositionInitialized)
    {
        MessageBuffer buffer;
        buffer << sb47::PlayerPositionAndLook(m_playerX, m_playerY, m_playerZ, m_playerYaw, m_playerPitch, true);

        emit outboundMessage(buffer.getAllBytes());
    }
}

//----------------------------------------------------------------------------//
