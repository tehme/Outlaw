#ifndef CHUNKDATA_HPP
#define CHUNKDATA_HPP

#include <vector>

//----------------------------------------------------------------------------//

class ChunkData
{
public:
    ChunkData();
    ChunkData(int xSize, int ySize, int zSize);
    ChunkData(const int * data, int xSize, int ySize, int zSize);

    bool isValid() const;
    int getXSize() const;
    int getYSize() const;
    int getZSize() const;
    int getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, int blockCode);

private:
    bool             m_isValid;
    int              m_xSize;
    int              m_ySize;
    int              m_zSize;
    std::vector<int> m_data;
};

//----------------------------------------------------------------------------//

#endif // CHUNKDATA_HPP
