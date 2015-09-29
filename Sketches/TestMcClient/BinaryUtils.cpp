#include "BinaryUtils.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

//----------------------------------------------------------------------------//

void BinaryUtils::dumpHex(const char * bytes, int size)
{
    int nRows = size / 16;
    if(size % 16 != 0)
        nRows += 1;

    for(int rowNumber = 0; rowNumber < nRows; ++rowNumber)
    {
        const int offset = rowNumber * 16;
        const int rowSize = std::min(size - offset, 16);

        if(offset == size)
        {
            std::cout << "Ololo!" << std::endl;
        }

        for(int colNumber = 0; colNumber < rowSize; ++colNumber)
        {
            uchar c = uchar(bytes[offset + colNumber]);
            std::cout << std::hex << std::setfill('0') << std::setw(2) << uint(c) << ' ';
        }
        if(16 - rowSize > 0)
        {
            std::cout << std::setfill(' ') << std::setw((16 - rowSize) * 3) << "";
        }

        std::cout << ' ';

        for(int colNumber = 0; colNumber < rowSize; ++colNumber)
        {
            char c = bytes[offset + colNumber];
            std::cout << (isprint(c) ? c : '.');
        }
        if(16 - rowSize > 0)
        {
            std::cout << std::setfill(' ') << std::setw(16 - rowSize) << "";
        }

        std::cout << std::endl;
    }
}

void BinaryUtils::dumpHex(const QByteArray & byteArray)
{
    dumpHex(byteArray.constData(), byteArray.size());
}
