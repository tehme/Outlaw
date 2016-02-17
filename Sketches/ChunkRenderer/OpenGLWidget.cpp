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
    m_upSpeed(0.0f),
    m_cameraPosition(0.0f, 0.0f, 10.0f),
    m_cameraFront(0.0f, 0.0f, -1.0f),
    m_yawRotation(0.0f),
    m_pitchRotation(0.0f)
{
    this->setMouseTracking(true);

    int chunkData[] =
    {
        1, 1, 1,  1, 1, 1,  1, 1, 1,
        1, 1, 1,  1, 1, 1,  1, 1, 1,
        1, 0, 1,  0, 0, 0,  1, 0, 1
    };

    m_chunkData = ChunkData(chunkData, 3, 3, 3);
    m_chunkMesh = ChunkMesh::createFromChunkData(m_chunkData);
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


    m_vertexBuffer.create();
    m_indexBuffer.create();

    m_vertexArray.create();
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(m_chunkMesh.getVertices().data(), m_chunkMesh.getVertices().size() * sizeof(GLfloat));

    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.allocate(m_chunkMesh.getIndices().data(), m_chunkMesh.getIndices().size() * sizeof(GLuint));

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

    static const QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    QVector3D cameraRight = QVector3D::crossProduct(m_cameraFront, cameraUp).normalized();

    m_cameraPosition += m_forwardSpeed * 50 / 1000 * m_cameraFront;
    m_cameraPosition += m_strafeSpeed * 50 / 1000 * cameraRight;
    m_cameraPosition += m_upSpeed * 50 / 1000 * cameraUp;

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, cameraUp);

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    m_shaderProgram.setUniformValue("viewMatrix", viewMatrix);
    m_shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);

    m_glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!m_chunkMesh.isEmpty())
    {
        m_shaderProgram.setUniformValue("modelMatrix", QMatrix4x4());
        m_glFuncs->glDrawElements(GL_TRIANGLES, m_chunkMesh.getNumberOfElements(), GL_UNSIGNED_INT, 0);
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
    if(keyEvent->key() == Qt::Key_Space)
    {
        m_upSpeed += 2.0f;
    }
    if(keyEvent->key() == Qt::Key_Shift)
    {
        m_upSpeed -= 2.0f;
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
    if(keyEvent->key() == Qt::Key_Space)
    {
        m_upSpeed -= 2.0f;
    }
    if(keyEvent->key() == Qt::Key_Shift)
    {
        m_upSpeed += 2.0f;
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * mouseEvent)
{
    // Ignore events that return mouse to center.
    if(mouseEvent->pos() == QPoint(this->width() / 2, this->height() / 2))
    {
        return;
    }

    const int xDelta = mouseEvent->x() - (this->width() / 2);
    const int yDelta = mouseEvent->y() - (this->height() / 2);
    static const float sensitivity = 0.25f;
    m_yawRotation += xDelta * sensitivity;
    m_pitchRotation -= yDelta * sensitivity;

    if(m_pitchRotation < -89.0f)
    {
        m_pitchRotation = -89.0f;
    }
    if(m_pitchRotation > 89.0f)
    {
        m_pitchRotation = 89.0f;
    }

    const float yawRad = m_yawRotation * 3.14 / 180;
    const float pitchRad = m_pitchRotation * 3.14 / 180;

    m_cameraFront.setX(std::cos(pitchRad) * std::cos(yawRad));
    m_cameraFront.setY(std::sin(pitchRad));
    m_cameraFront.setZ(std::cos(pitchRad) * std::sin(yawRad));

    QCursor cursor = this->cursor();
    cursor.setPos(mapToGlobal(QPoint(this->width() / 2, this->height() / 2)));
    setCursor(cursor);
}

//----------------------------------------------------------------------------//
