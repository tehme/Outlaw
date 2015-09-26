#ifndef BINARYUTILS_HPP
#define BINARYUTILS_HPP

#include <QByteArray>


class BinaryUtils
{
public:
    static void dumpHex(const char * bytes, int size);
    static void dumpHex(const QByteArray & byteArray);
};

#endif // BINARYUTILS_HPP
