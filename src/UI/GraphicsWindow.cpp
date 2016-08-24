#include "GraphicsWindow.h"

#include <Core/Global.h>

#include <QtMath>
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
    delete cube;
}

void GraphicsWindow::initialize()
{
    initializeOpenGLFunctions();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    camera.setPerspective(45.0f, (float)width() / (float)height(), 0.1f, 100.0f);

    if(!mainView)
    {
        triangle = new Triangle(this);
        triangle->Initialize(this);

        cube = new Cube(this);
        cube->Initialize(this);
    }
}

void GraphicsWindow::render()
{
    GraphicsWindow* window = mainView ? mainView : this;

    //camera.lookAtWithRadius(frame, 10.0f);

    window->cube->Draw2(camera.getView(), camera.getProjection());
    //window->cube->Draw(mvp);
    //window->triangle->Draw(view*camera.getProjection());
}

void GraphicsWindow::resizeGL(int width, int height)
{
    GLWindow::resizeGL(width, height);

    camera.setPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent* event)
{
    QPoint diff = lastPos - event->pos();

    if(event->buttons() & Qt::LeftButton)
    {
        camera.updateCenter(QVector2D(diff));
        //camera.rotate(diff.x(), QVector3D(0.0f, 1.0f, 0.0f));
        //camera.rotate(diff.y(), camera.getRight2());
    }

    lastPos = event->pos();
}

void GraphicsWindow::keyPressEvent(QKeyEvent* event)
{
    camera.processKey(event->key(), getDeltaTime());

    switch(event->key())
    {
        case Qt::Key_Plus:
            frame += 0.1;
            break;
    }
}
