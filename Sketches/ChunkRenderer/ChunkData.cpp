#include "ChunkData.hpp"

//----------------------------------------------------------------------------//

ChunkData::ChunkData() :
    m_isValid(false),
    m_xSize(0),
    m_ySize(0),
    m_zSize(0)
{
}

ChunkData::ChunkData(int xSize, int ySize, int zSize) :
    m_isValid(true),
    m_xSize(xSize),
    m_ySize(ySize),
    m_zSize(zSize)
{
    const int fullSize = xSize * ySize * zSize;
    m_data.resize(fullSize, 0);
}

ChunkData::ChunkData(const int * data, int xSize, int ySize, int zSize) :
    m_isValid(true),
    m_xSize(xSize),
    m_ySize(ySize),
    m_zSize(zSize)
{
    const int fullSize = xSize * ySize * zSize;
    m_data.assign(data, data + fullSize);
}

int ChunkData::getXSize() const
{
    return m_xSize;
}

int ChunkData::getYSize() const
{
    return m_ySize;
}

int ChunkData::getZSize() const
{
    return m_zSize;
}

int ChunkData::getBlock(int x, int y, int z) const
{
    const int blockPosition = y * m_xSize * m_zSize + z * m_xSize + x;

    if(blockPosition < 0 || blockPosition >= m_data.size())
    {
        return -1;
    }

    return m_data.at(blockPosition);
}

void ChunkData::setBlock(int x, int y, int z, int blockCode)
{
    const int blockPosition = y * m_xSize * m_zSize + z * m_xSize + x;

    if(blockPosition < 0 || blockPosition >= m_data.size())
    {
        return;
    }

    m_data[blockPosition] = blockCode;
}

//----------------------------------------------------------------------------//
