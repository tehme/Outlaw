#ifndef VARINT_HPP
#define VARINT_HPP

//----------------------------------------------------------------------------//

class MessageBuffer;

//----------------------------------------------------------------------------//

class VarInt
{
public:
    VarInt(int number = 0);
    ~VarInt();

    int getNumber() const;
    void setNumber(int number);

private:
    int m_number;
};

//----------------------------------------------------------------------------//

MessageBuffer & operator << (MessageBuffer & buffer, const VarInt & vi);

MessageBuffer & operator >> (MessageBuffer & buffer, VarInt & vi);

//----------------------------------------------------------------------------//

#endif // VARINT_HPP
