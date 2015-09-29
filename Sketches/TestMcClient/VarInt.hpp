#ifndef VARINT_HPP
#define VARINT_HPP

//----------------------------------------------------------------------------//

class MessageBuffer;

//----------------------------------------------------------------------------//

class VarInt
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

MessageBuffer & operator << (MessageBuffer & buffer, const VarInt & vi);

MessageBuffer & operator >> (MessageBuffer & buffer, VarInt & vi);

//----------------------------------------------------------------------------//

#endif // VARINT_HPP
