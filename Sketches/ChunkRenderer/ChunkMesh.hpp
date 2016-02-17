#ifndef CHUNKMESH_HPP
#define CHUNKMESH_HPP

#include <vector>
#include <GL/gl.h>

//----------------------------------------------------------------------------//

class ChunkData;

//----------------------------------------------------------------------------//

class ChunkMesh
{
public:
    ChunkMesh();

    static ChunkMesh createFromChunkData(const ChunkData & chunkData);

    bool isValid() const;
    bool isEmpty() const;
    int getNumberOfElements() const;
    const std::vector<GLfloat> & getVertices() const;
    const std::vector<GLuint> & getIndices() const;

private:
    enum class BlockFace
    {
        // North is -z
        // East is +x
        // Blocks go as YZX
        North,
        South,
        West,
        East,
        Bottom,
        Top
    };

private:
    static bool isAirNearBlockFace(const ChunkData & chunkData, int blockX, int blockY, int blockZ, BlockFace face);

private:
    bool                 m_isValid;
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint>  m_indices;
};

//----------------------------------------------------------------------------//

#endif // CHUNKMESH_HPP
