#include "MessageBuffer.hpp"

//----------------------------------------------------------------------------//

MessageBuffer::MessageBuffer() :
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

const QByteArray & MessageBuffer::getAllBytes()
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

int MessageBuffer::getOffset() const
{
    return m_offset;
}

void MessageBuffer::setOffset(int offset)
{
    m_offset = (offset <= m_bytes.size()) ? offset : m_bytes.size();
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

void MessageBuffer::writeBytesToBuffer(const QByteArray &bytes)
{
    m_bytes.append(bytes);
}

void MessageBuffer::writeBytesToBuffer(const char * bytes, int size)
{
    m_bytes.append(bytes, size);
}

//----------------------------------------------------------------------------//

MessageBuffer & operator << (MessageBuffer & buffer, const QString & s)
{
    QByteArray utf8Bytes = s.toUtf8();
    buffer << VarInt(utf8Bytes.size());
    buffer.writeBytesToBuffer(utf8Bytes);

    return buffer;
}

//----------------------------------------------------------------------------//

MessageBuffer & operator >> (MessageBuffer & buffer, QString & s)
{
    const int startingOffset = buffer.getOffset();

    try
    {
        VarInt size;
        buffer >> size;

        QByteArray utf8Bytes = buffer.readBytesFromBuffer(size.getNumber());

        s = QString::fromUtf8(utf8Bytes);
    }
    catch(...)
    {
        buffer.setOffset(startingOffset);
        throw;
    }

    return buffer;
}

//----------------------------------------------------------------------------//
