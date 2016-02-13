#include "OpenGLWidget.hpp"
#include <QOpenGLContext>
#include <QMatrix4x4>
#include <QDebug>

//----------------------------------------------------------------------------//

OpenGLWidget::OpenGLWidget(QWidget * parent) :
    QOpenGLWidget(parent),
    m_glFuncs(nullptr),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
}

OpenGLWidget::~OpenGLWidget()
{
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
        -0.5f, -0.5f, +0.0f,
        -0.5f, +0.5f, +0.0f,
        +0.5f, +0.5f, +0.0f,
        +0.5f, -0.5f, +0.0f
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
    m_glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_glFuncs->glEnableVertexAttribArray(0);

    m_vertexArray.release();

    m_vertexBuffer.release();
    m_indexBuffer.release();


    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/VertexShader.glsl");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/FragmentShader.glsl");

    m_glFuncs->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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

    QMatrix4x4 viewMatrix;
    viewMatrix.translate(0.0f, 0.0f, -2.0f);

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(width()) / height(), 0.1f, 10.0f);

    m_shaderProgram.setUniformValue("viewMatrix", viewMatrix);
    m_shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);

    m_glFuncs->glClear(GL_COLOR_BUFFER_BIT);
    m_glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_vertexArray.release();
    m_shaderProgram.release();
}

//----------------------------------------------------------------------------//
