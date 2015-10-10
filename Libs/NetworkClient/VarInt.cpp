#include "VarInt.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <QByteArray>
#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

VarInt::VarInt(int value) :
    m_value(value)
{
}

VarInt::~VarInt()
{
}

int VarInt::getValue() const
{
    return m_value;
}

void VarInt::setValue(int value)
{
    m_value = value;
}

// TODO: test this algorithm.
int VarInt::getSizeAsArray() const
{
    if(m_value < 0)
        return 5;

//    int value = m_value;
    for(int i = 0; i <= 4; ++i)
    {
        uint mask = (0b01111111 << (i * 7));
        if(m_value <= mask)
            return i + 1;

//        value -= mask;
    }

    std::cout << "Value is " << m_value << std::endl;
    assert(!"should never reach here");
    return 0;
}

//----------------------------------------------------------------------------//

MessageBuffer & operator << (MessageBuffer & buffer, const VarInt & vi)
{
    // TODO: maybe, this function is worth being moved to VarInt class to form general VarInt buffers.

    int number = vi.getValue();

    for(int currentBitOffset = 0; currentBitOffset < 32; currentBitOffset += 7)
    {
        char byte = (number >> currentBitOffset) & 0b01111111;

        // Erase read value to check if we need to move further.
        // If number is not 0, write continue bit and go on. Else, stop.
        number -= (byte << currentBitOffset);
        const bool needToContinue = (number != 0);

        if(needToContinue)
        {
            byte |= 0b10000000;
        }

        buffer << byte;

        if(!needToContinue)
        {
            break;
        }
    }

    return buffer;
}

MessageBuffer & operator >> (MessageBuffer & buffer, VarInt & vi)
{
    int resultNumber = 0;
    int currentBitOffset = 0;
    const int bufferOffset = buffer.getOffset();
    QByteArray bytes;

    while(true)
    {
        bytes = buffer.readBytesFromBuffer(1);
        if(bytes.isEmpty())
        {
            //TODO: is this enough for strong guarantee?
            buffer.setOffset(bufferOffset);
            throw MessageBuffer::NotEnoughBytesException();
        }

        const char byte = bytes.at(0);

        resultNumber |= ((byte & 0b01111111) << currentBitOffset);

        if(!(byte & 0b10000000))
            break;

        currentBitOffset += 7;
    }

    vi.setValue(resultNumber);

    return buffer;
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
