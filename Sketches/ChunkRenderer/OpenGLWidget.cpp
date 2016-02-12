#include "OpenGLWidget.hpp"
#include <QOpenGLContext>
#include <QDebug>

//----------------------------------------------------------------------------//

OpenGLWidget::OpenGLWidget(QWidget * parent) :
    QOpenGLWidget(parent),
    m_glFuncs(nullptr)
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
        +0.5f, -0.5f, +0.0f,
        +0.0f, +0.5f, +0.0f
    };

    m_vertexBuffer.create();

    m_vertexArray.create();
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(vertices, sizeof(vertices));

    // Vertex coordinates attribute
    m_glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_glFuncs->glEnableVertexAttribArray(0);

    m_vertexArray.release();

    m_vertexBuffer.release();


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

    m_glFuncs->glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram.bind();
    m_vertexArray.bind();

    m_glFuncs->glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vertexArray.release();
    m_shaderProgram.release();
}

//----------------------------------------------------------------------------//
