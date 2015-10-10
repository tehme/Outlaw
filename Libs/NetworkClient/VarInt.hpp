#ifndef VARINT_HPP
#define VARINT_HPP

#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class MessageBuffer;

class NETWORK_CLIENT_EXPORTED VarInt
{
public:
    VarInt(int value = 0);
    ~VarInt();

    int getValue() const;
    void setValue(int value);

    int getSizeAsArray() const;

private:
    int m_value;
};


//----------------------------------------------------------------------------//

NETWORK_CLIENT_EXPORTED MessageBuffer & operator << (MessageBuffer & buffer, const VarInt & vi);

NETWORK_CLIENT_EXPORTED MessageBuffer & operator >> (MessageBuffer & buffer, VarInt & vi);

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // VARINT_HPP
