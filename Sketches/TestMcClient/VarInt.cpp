#include "VarInt.hpp"
#include <QByteArray>
#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

VarInt::VarInt(int number) :
    m_number(number)
{
}

VarInt::~VarInt()
{
}

int VarInt::getNumber() const
{
    return m_number;
}

void VarInt::setNumber(int number)
{
    m_number = number;
}

//----------------------------------------------------------------------------//

MessageBuffer & operator << (MessageBuffer & buffer, const VarInt & vi)
{
    // TODO: maybe, this function is worth being moved to VarInt class to form general VarInt buffers.

    int number = vi.getNumber();

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
            throw NotEnoughBytesException();
        }

        const char byte = bytes.at(0);

        resultNumber |= ((byte & 0b01111111) << currentBitOffset);

        if(!(byte & 0b10000000))
            break;

        currentBitOffset += 7;
    }

    vi.setNumber(resultNumber);

    return buffer;
}

//----------------------------------------------------------------------------//
