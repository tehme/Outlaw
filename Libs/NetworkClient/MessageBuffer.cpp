#include "MessageBuffer.hpp"
#include "VarInt.hpp"

//----------------------------------------------------------------------------//

namespace NetworkClient {

//----------------------------------------------------------------------------//

MessageBuffer::MessageBuffer() :
    m_offset(0)
{
}

MessageBuffer::MessageBuffer(const QByteArray & byteArray) :
    m_bytes(byteArray),
    m_offset(0)
{
}

MessageBuffer::~MessageBuffer()
{
}

bool MessageBuffer::canReadBytes(int nBytes) const
{
    return (m_bytes.size() - m_offset) >= nBytes;
}

int MessageBuffer::getSize() const
{
    const int size = m_bytes.size() - m_offset;
    return (size >= 0) ? size : 0;
}

const QByteArray & MessageBuffer::getAllBytes() const
{
    return m_bytes;
}

void MessageBuffer::clear(int nBytes)
{
    if(nBytes < 0 || nBytes >= m_bytes.size()) // all
    {
        nBytes = m_bytes.size();
        m_bytes.clear();
    }
    else
    {
        m_bytes.remove(0, nBytes);
    }

    m_offset -= nBytes;
    if(m_offset < 0)
        m_offset = 0;
}

void MessageBuffer::clearToOffset()
{
    clear(m_offset);
}

int MessageBuffer::getOffset() const
{
    return m_offset;
}

void MessageBuffer::setOffset(int offset)
{
    m_offset = (offset <= m_bytes.size()) ? offset : m_bytes.size();
}

void MessageBuffer::moveOffset(int relativeOffset)
{
    setOffset(m_offset + relativeOffset);
}

//char MessageBuffer::readByteFromBuffer(bool moveOffset)
//{

//}

QByteArray MessageBuffer::readBytesFromBuffer(int nBytes, bool moveOffset)
{
    QByteArray bytes = m_bytes.mid(m_offset, nBytes);

    if(moveOffset)
        setOffset(m_offset + bytes.size());

    return bytes;
}

void MessageBuffer::writeBytesToBuffer(const QByteArray & bytes)
{
    m_bytes.append(bytes);
}

void MessageBuffer::writeBytesToBuffer(const char * bytes, int size)
{
    m_bytes.append(bytes, size);
}

void MessageBuffer::writeBytesToBuffer(const unsigned char * bytes, int size)
{
    m_bytes.append(reinterpret_cast<const char *>(bytes), size);
}

//----------------------------------------------------------------------------//

MessageBuffer & operator << (MessageBuffer & buffer, float src)
{
    quint32 srcAsUint = reinterpret_cast<const quint32 &>(src);
    buffer << srcAsUint;

    return buffer;
}

MessageBuffer & operator << (MessageBuffer & buffer, double src)
{
    quint64 srcAsUint64 = reinterpret_cast<const quint64 &>(src);
    buffer << srcAsUint64;

    return buffer;
}

MessageBuffer & operator << (MessageBuffer & buffer, const QString & src)
{
    QByteArray utf8Bytes = src.toUtf8();
    buffer << VarInt(utf8Bytes.size());
    buffer.writeBytesToBuffer(utf8Bytes);

    return buffer;
}

MessageBuffer & operator << (MessageBuffer & buffer, const QUuid & src)
{
    buffer << src.data1 << src.data2 << src.data3;
    buffer.writeBytesToBuffer(src.data4, sizeof(src.data4));

    return buffer;
}

//----------------------------------------------------------------------------//

MessageBuffer & operator >> (MessageBuffer & buffer, float & dst)
{
    quint32 dstAsUint;
    buffer >> dstAsUint;

    dst = reinterpret_cast<const float &>(dstAsUint);

    return buffer;
}

MessageBuffer & operator >> (MessageBuffer & buffer, double & dst)
{
    quint64 dstAsUint64;
    buffer >> dstAsUint64;

    dst = reinterpret_cast<const double &>(dstAsUint64);

    return buffer;
}

MessageBuffer & operator >> (MessageBuffer & buffer, QString & dst)
{
    const int startingOffset = buffer.getOffset();

    try
    {
        VarInt size;
        buffer >> size;

        QByteArray utf8Bytes = buffer.readBytesFromBuffer(size.getValue());

        dst = QString::fromUtf8(utf8Bytes);
    }
    catch(...)
    {
        buffer.setOffset(startingOffset);
        throw;
    }

    return buffer;
}

MessageBuffer & operator >> (MessageBuffer & buffer, QUuid & dst)
{
    const int startingOffset = buffer.getOffset();

    try
    {
        uint l;
        ushort w1, w2;
        uchar b1, b2, b3, b4, b5, b6, b7, b8;

        buffer >> l >> w1 >> w2 >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8;
        dst = QUuid(l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
    }
    catch(...)
    {
        buffer.setOffset(startingOffset);
        throw;
    }

    return buffer;
}

//----------------------------------------------------------------------------//

} // namespace NetworkClient

//----------------------------------------------------------------------------//
