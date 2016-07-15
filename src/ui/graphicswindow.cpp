#include "graphicswindow.h"

#include <QDebug>

GraphicsWindow::GraphicsWindow()
: GLWindow()
, mainView(NULL)
{
}

GraphicsWindow::GraphicsWindow(GraphicsWindow* view)
: GLWindow(view->context())
, mainView(view)
, triangle(NULL)
{

}

GraphicsWindow::~GraphicsWindow()
{
    delete triangle;
}

void GraphicsWindow::initialize()
{
    glEnable(GL_CULL_FACE);

    mvp.perspective(60.0f, (float)width() / (float)height(), 0.1f, 100.0f);

    if(!mainView)
    {
        triangle = new Triangle(this);
        triangle->Initialize();

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
    if(mainView)
    {
        Triangle* triangle = mainView->getTriangle();
        triangle->Draw(mvp);

        return;
    }

    triangle->Draw(mvp);
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent* event)
{

}
