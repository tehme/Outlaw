#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include <exception>
#include <QByteArray>
#include <QString>
#include <QtEndian>
#include "NetworkClientGeneral.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

class NETWORK_CLIENT_EXPORTED MessageBuffer
{
public:
    class NotEnoughBytesException : public std::exception {};

public:
    MessageBuffer();
    MessageBuffer(const QByteArray & byteArray);
    ~MessageBuffer();

    bool canReadBytes(int nBytes) const;
    int getSize() const;
    const QByteArray & getAllBytes() const;

    void clear(int nBytes= -1);
    void clearToOffset();

    int getOffset() const;
    void setOffset(int offset);
    void moveOffset(int relativeOffset);

//    char readByteFromBuffer(bool moveOffset = true);
    QByteArray readBytesFromBuffer(int nBytes, bool moveOffset = true);
    void writeBytesToBuffer(const QByteArray & bytes);
    void writeBytesToBuffer(const char * bytes, int size);

private:
    QByteArray m_bytes;
    int m_offset;
};

//----------------------------------------------------------------------------//

template<typename IntType>
MessageBuffer & operator << (MessageBuffer & buffer, IntType src);

// TODO: float/double (mb IntType version will fit, if they store floats big endian)
NETWORK_CLIENT_EXPORTED MessageBuffer & operator << (MessageBuffer & buffer, float src);

NETWORK_CLIENT_EXPORTED MessageBuffer & operator << (MessageBuffer & buffer, const QString & src);

//----------------------------------------------------------------------------//

template<typename IntType>
MessageBuffer & operator >> (MessageBuffer & buffer, IntType & dst);

NETWORK_CLIENT_EXPORTED MessageBuffer & operator >> (MessageBuffer & buffer, float & dst);

NETWORK_CLIENT_EXPORTED MessageBuffer & operator >> (MessageBuffer & buffer, QString & dst);

//----------------------------------------------------------------------------//

template<typename IntType>
MessageBuffer & operator << (MessageBuffer & buffer, IntType src)
{
    const int varSize = sizeof(src);

    char srcAsBytes[varSize];
    qToBigEndian(src, reinterpret_cast<uchar *>(srcAsBytes));

    buffer.writeBytesToBuffer(srcAsBytes, varSize);

    return buffer;
}

//----------------------------------------------------------------------------//

template<typename IntType>
MessageBuffer & operator >> (MessageBuffer & buffer, IntType & dst)
{
    const int varSize = sizeof(dst);

    QByteArray bytes = buffer.readBytesFromBuffer(varSize);
    if(bytes.size() < varSize)
    {
        buffer.setOffset(buffer.getOffset() - bytes.size());
        throw MessageBuffer::NotEnoughBytesException();
    }

    dst = qFromBigEndian<IntType>(reinterpret_cast<const uchar *>(bytes.constData()));

    return buffer;
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//

#endif // MESSAGEBUFFER_H
