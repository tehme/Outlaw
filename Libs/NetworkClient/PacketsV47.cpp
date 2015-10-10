#include "PacketsV47.hpp"
#include "MessageBuffer.hpp"
#include "VarInt.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

namespace PacketsV47 {

//----------------------------------------------------------------------------//

namespace Clientbound {

//----------------------------------------------------------------------------//

PlayerPositionAndLook::PlayerPositionAndLook(bool readCode) :
    PlayerPositionAndLook(0.0, 0.0, 0.0, 0.0f, 0.0f, 0, readCode)
{
}

PlayerPositionAndLook::PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, quint8 flags, bool readCode) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_yaw(yaw),
    m_pitch(pitch),
    m_flags(flags), // TODO: detailed flags
    m_readCode(readCode)
{
}

MessageBuffer & operator >> (MessageBuffer & buffer, PlayerPositionAndLook & dst)
{
    if(dst.m_readCode)
    {
        VarInt code;
        buffer >> code;
        if(code.getValue() != dst.m_code)
        {
            buffer.moveOffset(-code.getSizeAsArray());
            throw std::exception(); // TODO: own exception for trying to read wrong message
        }
    }

    buffer
        >> dst.m_x
        >> dst.m_y
        >> dst.m_z
        >> dst.m_yaw
        >> dst.m_pitch
        >> dst.m_flags;

    return buffer;
}

//----------------------------------------------------------------------------//

} // namespace Clientbound

//----------------------------------------------------------------------------//

namespace Serverbound {

//----------------------------------------------------------------------------//

PlayerPositionAndLook::PlayerPositionAndLook() :
    PlayerPositionAndLook(0.0, 0.0, 0.0, 0.0f, 0.0f, false)
{
}

PlayerPositionAndLook::PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, bool onGround) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_yaw(yaw),
    m_pitch(pitch),
    m_onGround(onGround)
{
}

MessageBuffer & operator << (MessageBuffer & buffer, const PlayerPositionAndLook & src)
{
    buffer
        << VarInt(src.m_code)
        << src.m_x
        << src.m_y
        << src.m_z
        << src.m_yaw
        << src.m_pitch
        << src.m_onGround;

    return buffer;
}

//----------------------------------------------------------------------------//

} // namespace Serverbound

//----------------------------------------------------------------------------//

} // namespace PacketsV47

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
