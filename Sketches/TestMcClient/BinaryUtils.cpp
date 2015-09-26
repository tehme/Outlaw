#include "BinaryUtils.hpp"
#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------//

void BinaryUtils::dumpHex(const char * bytes, int size)
{
    int nRows = size / 16;
    if(size % 16 != 0)
        nRows += 1;

    for(int rowNumber = 0; rowNumber <= nRows; ++rowNumber)
    {
        const int offset = rowNumber * 16;

        int rowSize = size - offset;
        if(rowSize > 16)
            rowSize = 16;

        for(int colNumber = 0; colNumber < rowSize; ++colNumber)
        {
            uchar c = uchar(bytes[offset + colNumber]);
            std::cout << std::hex << std::setfill('0') << std::setw(2) << uint(c) << ' ';
        }
        std::cout.write("   ", 16 - rowSize),

        std::cout << ' ';

        for(int colNumber = 0; colNumber < rowSize; ++colNumber)
        {
            char c = bytes[offset + colNumber];
            std::cout << (isprint(c) ? c : '.');
        }
        std::cout.write(" ", 16 - rowSize),

        std::cout << std::endl;
    }
}

void BinaryUtils::dumpHex(const QByteArray & byteArray)
{
    dumpHex(byteArray.constData(), byteArray.size());
}
