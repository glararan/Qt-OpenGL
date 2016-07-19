#include "graphicswindow.h"

#include "../core/global.h"

#include <QDebug>
#include <QMouseEvent>


GraphicsWindow::GraphicsWindow(QWindow* parent)
: GLWindow(this, GLMgr->context(), GLWindow::NoPartialUpdate, parent)
{
}

GraphicsWindow::GraphicsWindow(GraphicsWindow* view, QWindow* parent)
: GLWindow(this, GLMgr->context(), GLWindow::NoPartialUpdate, parent)
, mainView(view)
{

}

GraphicsWindow::~GraphicsWindow()
{
    delete triangle;
}

void GraphicsWindow::initialize()
{
    initializeOpenGLFunctions();

    glEnable(GL_CULL_FACE);

    mvp.perspective(60.0f, (float)width() / (float)height(), 0.1f, 100.0f);

    if(!mainView)
    {
        triangle = new Triangle(this);
        triangle->Initialize(this);

        mvp.translate(0, 0, -2);
        mvp.rotate(30.0f, 0, 1, 0);
    }
    else
    {
        mvp.translate(0, 0, -4);
        mvp.rotate(0.0f, 0, 1, 0);
    }
}

void GraphicsWindow::render()
{
    GraphicsWindow* window = mainView ? mainView : this;

    window->triangle->Draw(mvp, mainView ? true : false);
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent* event)
{
    QPoint diff = lastPos - event->pos();

    mvp.rotate(diff.x() * -1, 0, 1, 0);

    lastPos = event->pos();
}
