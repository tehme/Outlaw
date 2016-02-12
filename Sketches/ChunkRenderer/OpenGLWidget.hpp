#ifndef OPENGLWIDGET_HPP
#define OPENGLWIDGET_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

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

private:
    QOpenGLFunctions_3_3_Core * m_glFuncs;
};

//----------------------------------------------------------------------------//

#endif // OPENGLWIDGET_HPP
