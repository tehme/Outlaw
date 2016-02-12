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
}

void OpenGLWidget::resizeGL(int width, int height)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
}

void OpenGLWidget::paintGL()
{
}

//----------------------------------------------------------------------------//
