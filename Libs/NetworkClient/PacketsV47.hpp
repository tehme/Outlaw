#ifndef PACKETSV47_HPP
#define PACKETSV47_HPP

#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class MessageBuffer;

//----------------------------------------------------------------------------//

namespace PacketsV47 {

//----------------------------------------------------------------------------//

namespace Clientbound {

//----------------------------------------------------------------------------//

struct NETWORK_CLIENT_EXPORTED PlayerPositionAndLook
{
    PlayerPositionAndLook(bool readCode = false);
    PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, quint8 flags, bool readCode = false);

    static const int m_code = 0x08;
    double m_x;
    double m_y;
    double m_z;
    float  m_yaw;
    float  m_pitch;
    quint8 m_flags;
    bool   m_readCode;
};

NETWORK_CLIENT_EXPORTED MessageBuffer & operator >> (MessageBuffer & buffer, PlayerPositionAndLook & dst);

//----------------------------------------------------------------------------//

} // namespace Clientbound

//----------------------------------------------------------------------------//

namespace Serverbound {

//----------------------------------------------------------------------------//

struct NETWORK_CLIENT_EXPORTED PlayerPositionAndLook
{
    PlayerPositionAndLook();
    PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, bool onGround);

    static const int m_code = 0x06;
    double m_x;
    double m_y;
    double m_z;
    float  m_yaw;
    float  m_pitch;
    bool   m_onGround;
};

// We will not read this from buffer (yet)
NETWORK_CLIENT_EXPORTED MessageBuffer & operator << (MessageBuffer & buffer, const PlayerPositionAndLook & src);

//----------------------------------------------------------------------------//

} // namespace Serverbound

//----------------------------------------------------------------------------//

} // namespace PacketsV47

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // PACKETSV47_HPP
