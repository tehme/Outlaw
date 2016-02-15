#include "OpenGLWidget.hpp"
#include <vector>
#include <QOpenGLContext>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QDebug>

//----------------------------------------------------------------------------//

OpenGLWidget::OpenGLWidget(QWidget * parent) :
    QOpenGLWidget(parent),
    m_glFuncs(nullptr),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(QOpenGLTexture::Target2D),
    m_forwardSpeed(0.0f),
    m_strafeSpeed(0.0f),
    m_xDistance(0.0f),
    m_zDistance(-3.0f),
    m_yawRotation(0.0f)
{
    this->setMouseTracking(true);

    int chunkData[] =
    {
        1, 1, 1,  1, 1, 1,  1, 1, 1,
        1, 1, 1,  1, 1, 1,  1, 1, 1,
        1, 0, 1,  0, 0, 0,  1, 0, 1
    };

    m_chunkData = ChunkData(chunkData, 3, 3, 3);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    m_texture.destroy();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    m_glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    if(!m_glFuncs || !m_glFuncs->initializeOpenGLFunctions())
    {
        qWarning() << "ChunkRenderer: failed to get OpenGL functions.";
        m_glFuncs = nullptr;
    }


    GLfloat vertices[] =
    {
        -0.5f, -0.5f, +0.0f,  0.0f, 0.0f,
        -0.5f, +0.5f, +0.0f,  0.0f, 1.0f,
        +0.5f, +0.5f, +0.0f,  1.0f, 1.0f,
        +0.5f, -0.5f, +0.0f,  1.0f, 0.0f
    };

    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_vertexBuffer.create();
    m_indexBuffer.create();

    m_vertexArray.create();
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(vertices, sizeof(vertices));

    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.allocate(indices, sizeof(indices));

    // Vertex coordinates attribute
    m_glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    m_glFuncs->glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    m_glFuncs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    m_glFuncs->glEnableVertexAttribArray(1);

    m_vertexArray.release();

    m_vertexBuffer.release();
    m_indexBuffer.release();


    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/VertexShader.glsl");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/FragmentShader.glsl");


    m_texture.setData(QImage(":/textures/Texture.png").mirrored(false, true));
    m_texture.setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);


    m_glFuncs->glEnable(GL_DEPTH_TEST);
    m_glFuncs->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    m_updateTimer.setInterval(50);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer.start();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);

    if(!m_glFuncs)
    {
        return;
    }

    m_glFuncs->glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL()
{
    if(!m_glFuncs)
    {
        return;
    }

    m_shaderProgram.bind();
    m_vertexArray.bind();
    m_texture.bind();

    const double yawSin = std::sin(m_yawRotation * 3.14 / 180);
    const double yawCos = std::cos(m_yawRotation * 3.14 / 180);

    m_zDistance += m_forwardSpeed * 50 / 1000 * yawCos;
    m_xDistance += m_forwardSpeed * 50 / 1000 * yawSin;

    m_zDistance += m_strafeSpeed * 50 / 1000 * -yawSin;
    m_xDistance += m_strafeSpeed * 50 / 1000 * yawCos;

    QMatrix4x4 viewMatrix;
    viewMatrix.rotate(m_yawRotation, 0.0f, 1.0f, 0.0f);
    viewMatrix.translate(-m_xDistance, 0.0f, m_zDistance);

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    m_shaderProgram.setUniformValue("viewMatrix", viewMatrix);
    m_shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);

    m_glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int y = 0; y < m_chunkData.getYSize(); ++y)
    {
        for(int z = 0; z < m_chunkData.getZSize(); ++z)
        {
            for(int x = 0; x < m_chunkData.getYSize(); ++x)
            {
                const int blockCode = m_chunkData.getBlock(x, y, z);
                if(blockCode != 0)
                {
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::North))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x, y, z);
                        modelMatrix.rotate(180.0f, 0.0f, 1.0f, 0.0f);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::South))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x, y, z + 1);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::West))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x - 0.5f, y, z + 0.5f);
                        modelMatrix.rotate(-90.0f, 0.0f, 1.0f, 0.0f);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::East))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x + 0.5f, y, z + 0.5f);
                        modelMatrix.rotate(90.0f, 0.0f, 1.0f, 0.0f);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::Bottom))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x, y - 0.5f, z + 0.5);
                        modelMatrix.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    if(isAirNearBlockFace(m_chunkData, x, y, z, BlockFace::Top))
                    {
                        QMatrix4x4 modelMatrix;
                        modelMatrix.translate(x, y + 0.5f, z + 0.5);
                        modelMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);
                        m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);
                        m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                }
            }
        }
    }

    m_texture.release();
    m_vertexArray.release();
    m_shaderProgram.release();
}

void OpenGLWidget::keyPressEvent(QKeyEvent * keyEvent)
{
    if(keyEvent->isAutoRepeat())
    {
        return;
    }

    if(keyEvent->key() == Qt::Key_W)
    {
        m_forwardSpeed += 2.0f;
    }
    if(keyEvent->key() == Qt::Key_S)
    {
        m_forwardSpeed -= 2.0f;
    }
    if(keyEvent->key() == Qt::Key_A)
    {
        m_strafeSpeed -= 2.0f;
    }
    if(keyEvent->key() == Qt::Key_D)
    {
        m_strafeSpeed += 2.0f;
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent * keyEvent)
{
    if(keyEvent->isAutoRepeat())
    {
        return;
    }

    if(keyEvent->key() == Qt::Key_W)
    {
        m_forwardSpeed -= 2.0f;
    }
    if(keyEvent->key() == Qt::Key_S)
    {
        m_forwardSpeed += 2.0f;
    }
    if(keyEvent->key() == Qt::Key_A)
    {
        m_strafeSpeed += 2.0f;
    }
    if(keyEvent->key() == Qt::Key_D)
    {
        m_strafeSpeed -= 2.0f;
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    // Ignore events that return mouse to center.
    if(mouseEvent->pos() == QPoint(width() / 2, height() / 2))
    {
        return;
    }

    int xDelta = mouseEvent->x() - (width() / 2);
    static const float sensitivity = 0.25f;
    m_yawRotation += xDelta * sensitivity;

    QCursor cursor = this->cursor();
    cursor.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    setCursor(cursor);
}

bool OpenGLWidget::isAirNearBlockFace(
    const ChunkData & chunkData,
    int               blockX,
    int               blockY,
    int               blockZ,
    BlockFace         face)
{
    switch(face)
    {
    case BlockFace::North:  return blockZ == 0 || chunkData.getBlock(blockX, blockY, blockZ - 1) == 0;
    case BlockFace::South:  return blockZ == chunkData.getZSize() - 1 || chunkData.getBlock(blockX, blockY, blockZ + 1) == 0;
    case BlockFace::West:   return blockX == 0 || chunkData.getBlock(blockX - 1, blockY, blockZ) == 0;
    case BlockFace::East:   return blockX == chunkData.getXSize() - 1 || chunkData.getBlock(blockX + 1, blockY, blockZ) == 0;
    case BlockFace::Bottom: return blockY == 0 || chunkData.getBlock(blockX, blockY - 1, blockZ) == 0;
    case BlockFace::Top:    return blockY == chunkData.getYSize() - 1 || chunkData.getBlock(blockX, blockY + 1, blockZ) == 0;
    default:                return false;
    }
}

//----------------------------------------------------------------------------//
