#include "ChunkMesh.hpp"
#include "ChunkData.hpp"
#include <QDebug>

//----------------------------------------------------------------------------//

ChunkMesh::ChunkMesh() :
    m_isValid(false)
{
}

ChunkMesh ChunkMesh::createFromChunkData(const ChunkData & chunkData)
{
    ChunkMesh mesh;
    mesh.m_isValid = true;

    unsigned int indicesOffset = 0;

    for(int y = 0; y < chunkData.getYSize(); ++y)
    {
        for(int z = 0; z < chunkData.getZSize(); ++z)
        {
            for(int x = 0; x < chunkData.getXSize(); ++x)
            {
                const int blockCode = chunkData.getBlock(x, y, z);
                if(blockCode != 0)
                {
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::North))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                1.0f + x, 0.0f + y, 0.0f + z,  0.0f, 0.0f,
                                0.0f + x, 0.0f + y, 0.0f + z,  0.0f, 1.0f,
                                0.0f + x, 1.0f + y, 0.0f + z,  1.0f, 1.0f,
                                1.0f + x, 1.0f + y, 0.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::South))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                0.0f + x, 0.0f + y, 1.0f + z,  0.0f, 0.0f,
                                0.0f + x, 1.0f + y, 1.0f + z,  0.0f, 1.0f,
                                1.0f + x, 1.0f + y, 1.0f + z,  1.0f, 1.0f,
                                1.0f + x, 0.0f + y, 1.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::West))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                0.0f + x, 0.0f + y, 0.0f + z,  0.0f, 0.0f,
                                0.0f + x, 1.0f + y, 0.0f + z,  0.0f, 1.0f,
                                0.0f + x, 1.0f + y, 1.0f + z,  1.0f, 1.0f,
                                0.0f + x, 0.0f + y, 1.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::East))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                1.0f + x, 0.0f + y, 1.0f + z,  0.0f, 0.0f,
                                1.0f + x, 1.0f + y, 1.0f + z,  0.0f, 1.0f,
                                1.0f + x, 1.0f + y, 0.0f + z,  1.0f, 1.0f,
                                1.0f + x, 0.0f + y, 0.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::Bottom))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                0.0f + x, 0.0f + y, 0.0f + z,  0.0f, 0.0f,
                                0.0f + x, 0.0f + y, 1.0f + z,  0.0f, 1.0f,
                                1.0f + x, 0.0f + y, 1.0f + z,  1.0f, 1.0f,
                                1.0f + x, 0.0f + y, 0.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                    if(isAirNearBlockFace(chunkData, x, y, z, BlockFace::Top))
                    {
                        mesh.m_vertices.insert(
                            mesh.m_vertices.end(),
                            {
                                0.0f + x, 1.0f + y, 1.0f + z,  0.0f, 0.0f,
                                0.0f + x, 1.0f + y, 0.0f + z,  0.0f, 1.0f,
                                1.0f + x, 1.0f + y, 0.0f + z,  1.0f, 1.0f,
                                1.0f + x, 1.0f + y, 1.0f + z,  1.0f, 0.0f
                            }
                        );

                        mesh.m_indices.insert(
                            mesh.m_indices.end(),
                            {
                                0 + indicesOffset, 1 + indicesOffset, 2 + indicesOffset,
                                0 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
                            }
                        );

                        indicesOffset += 4;
                    }
                }
            }
        }
    }

    return mesh;
}

bool ChunkMesh::isValid() const
{
    return m_isValid;
}

bool ChunkMesh::isEmpty() const
{
    return m_vertices.empty();
}

int ChunkMesh::getNumberOfElements() const
{
    return m_indices.size();
}

const std::vector<GLfloat> & ChunkMesh::getVertices() const
{
    return m_vertices;
}

const std::vector<GLuint> & ChunkMesh::getIndices() const
{
    return m_indices;
}

bool ChunkMesh::isAirNearBlockFace(const ChunkData & chunkData, int blockX, int blockY, int blockZ, BlockFace face)
{
    switch(face)
    {
    case BlockFace::North:
        return blockZ == 0 || chunkData.getBlock(blockX, blockY, blockZ - 1) == 0;
    case BlockFace::South:
        return blockZ == chunkData.getZSize() - 1 || chunkData.getBlock(blockX, blockY, blockZ + 1) == 0;
    case BlockFace::West:
        return blockX == 0 || chunkData.getBlock(blockX - 1, blockY, blockZ) == 0;
    case BlockFace::East:
        return blockX == chunkData.getXSize() - 1 || chunkData.getBlock(blockX + 1, blockY, blockZ) == 0;
    case BlockFace::Bottom:
        return blockY == 0 || chunkData.getBlock(blockX, blockY - 1, blockZ) == 0;
    case BlockFace::Top:
        return blockY == chunkData.getYSize() - 1 || chunkData.getBlock(blockX, blockY + 1, blockZ) == 0;
    default:
        return false;
    }
}

//----------------------------------------------------------------------------//
