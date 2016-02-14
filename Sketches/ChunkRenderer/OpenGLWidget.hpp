#ifndef OPENGLWIDGET_HPP
#define OPENGLWIDGET_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTimer>
#include "ChunkData.hpp"

//----------------------------------------------------------------------------//

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget * parent = nullptr);
    virtual ~OpenGLWidget() override;

private:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

    virtual void keyPressEvent(QKeyEvent * keyEvent) override;
    virtual void keyReleaseEvent(QKeyEvent * keyEvent) override;
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent) override;

private:
    QOpenGLFunctions_3_3_Core * m_glFuncs;
    QOpenGLShaderProgram        m_shaderProgram;
    QOpenGLBuffer               m_vertexBuffer;
    QOpenGLBuffer               m_indexBuffer;
    QOpenGLVertexArrayObject    m_vertexArray;
    QOpenGLTexture              m_texture;

    float                       m_forwardSpeed;
    float                       m_strafeSpeed;
    float                       m_xDistance;
    float                       m_zDistance;
    float                       m_yawRotation;

    QTimer                      m_updateTimer;

    ChunkData                   m_chunkData;
};

//----------------------------------------------------------------------------//

#endif // OPENGLWIDGET_HPP
