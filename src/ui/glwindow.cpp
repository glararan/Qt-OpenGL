#include "glwindow.h"

#include <QCoreApplication>
#include <QDebug>

GLWindow::GLWindow()
: QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
{
}

GLWindow::GLWindow(QOpenGLContext* sharedContext)
: QOpenGLWindow(sharedContext, QOpenGLWindow::NoPartialUpdate)
{

}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    //connect(this, &GLWindow::frameSwapped, this, &GLWindow::renderNow); // <--- this is slow!

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    initialize();

    renderNow();
}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    render();

    painted = true;

    if(fpsTimer.isActive())
        ++fps;
}

void GLWindow::renderNow()
{
    if(!isExposed())
        return;

    if(!painted)
        return;

    painted = false;

    QOpenGLContext::currentContext()->makeCurrent(this);

    QOpenGLWindow::update();

    QOpenGLContext::currentContext()->swapBuffers(this);

    if(draw && !pendingUpdate)
    {
        pendingUpdate = false;

        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void GLWindow::resizeGL(int width, int height)
{
    const qreal pixelRatio = devicePixelRatio();

    glViewport(0, 0, width * pixelRatio, height * pixelRatio);
}

bool GLWindow::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::UpdateRequest:
            {
                pendingUpdate = false;

                renderNow();
            }
            break;
    }

    return QOpenGLWindow::event(event);
}

void GLWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    if(isExposed())
        renderNow();
}

void GLWindow::setFpsCounter(bool enable)
{
    if(enable)
    {
        connect(&fpsTimer, &QTimer::timeout, this, &GLWindow::fpsTimeout);

        fpsTimer.start(1000);
    }
    else
    {
        disconnect(&fpsTimer, &QTimer::timeout, this, &GLWindow::fpsTimeout);

        fpsTimer.stop();
    }
}

void GLWindow::fpsTimeout()
{
    qDebug() << "FPS: " << fps;

    fps = 0;
}

void GLWindow::initialize()
{

}

void GLWindow::render()
{

}
